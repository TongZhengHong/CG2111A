#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <cctype>
#include "packet.h"
#include "serial.h"
#include "serialize.h"
#include "constants.h"

#include <unistd.h>
#include <termios.h>

#define PORT_NAME			"/dev/ttyACM0"
#define BAUD_RATE			B9600

int exitFlag=0;
sem_t _xmitSema;

static bool send_status = false;

char getch() {
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
			perror ("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror ("tcsetattr ~ICANON");
	return (buf);
}

void handleError(TResult error) {
	switch(error) {
		case PACKET_BAD:
			printf("ERROR: Bad Magic Number\n");
			break;

		case PACKET_CHECKSUM_BAD:
			printf("ERROR: Bad checksum\n");
			break;

		default:
			printf("ERROR: UNKNOWN ERROR\n");
	}
}

void handleStatus(TPacket *packet) {
	printf("\n ------- ALEX STATUS REPORT ------- \n\n");
	printf("Left Forward Ticks:\t\t%d\n", packet->params[0]);
	printf("Right Forward Ticks:\t\t%d\n", packet->params[1]);
	printf("Left Reverse Ticks:\t\t%d\n", packet->params[2]);
	printf("Right Reverse Ticks:\t\t%d\n", packet->params[3]);
	printf("Left Forward Ticks Turns:\t%d\n", packet->params[4]);
	printf("Right Forward Ticks Turns:\t%d\n", packet->params[5]);
	printf("Left Reverse Ticks Turns:\t%d\n", packet->params[6]);
	printf("Right Reverse Ticks Turns:\t%d\n", packet->params[7]);
	printf("Forward Distance:\t\t%d\n", packet->params[8]);
	printf("Reverse Distance:\t\t%d\n", packet->params[9]);
	printf("\n---------------------------------------\n\n");
}

void handleColor(TPacket *packet) {
	uint32_t red = packet->params[0];
	uint32_t green = packet->params[1];
	uint32_t distance = packet->params[2];
	
	printf("\n --------- ALEX COLOR SENSOR --------- \n\n");
	printf("Red frequency:\t\t%d\n", red);
	printf("Green frequency:\t%d\n", green);
	printf("Distance:\t\t%d cm\n", distance);
	
	// Determine color
	const int COLOR_THRESHOLD = 10;
	const int DIST_THRESHOLD = 10;
	
	uint32_t diff = (red > green) ? red-green : green-red; // abs diff
	uint32_t large = (red > green) ? red : green; // max
	
	float percentDiff = (float) diff / large * 100.0;
	printf( "Percent diff:\t\t%0.2lf%\n", percentDiff);
	if (percentDiff >= COLOR_THRESHOLD and distance <= DIST_THRESHOLD) {
		if (red < green) printf("\nRED!\n");
		else printf("\nGREEN!\n");
	} else printf("\nNo color detected!\n");
	printf("\n--------------------------------------\n\n");
}

void handleDistance(TPacket *packet) {
	uint32_t distance = packet->params[0];
	printf("Ultrasonic Distance:\t\t%d cm\n", distance);
	
	const int DIST_THRESHOLD = 25;
	if (distance < DIST_THRESHOLD) printf("WALL NEARBY! SLOW DOWN!\n");
}

void handleResponse(TPacket *packet) {
	// The response code is stored in command
	switch(packet->command) {
		case RESP_OK:
			printf("Command OK\n");
			send_status = false;
			break;

		case RESP_STATUS:
			handleStatus(packet);
			break;
		
		case RESP_COLOR:
			handleColor(packet);
			break;
			
		case RESP_DIST:
			handleDistance(packet);
			break;

		default:
			printf("Arduino is confused\n");
	}
}

void handleErrorResponse(TPacket *packet) {
	// The error code is returned in command
	switch(packet->command) {
		case RESP_BAD_PACKET:
			printf("Arduino received bad magic number\n");
			break;

		case RESP_BAD_CHECKSUM:
			printf("Arduino received bad checksum\n");
			break;

		case RESP_BAD_COMMAND:
			printf("Arduino received bad command\n");
			break;

		case RESP_BAD_RESPONSE:
			printf("Arduino received unexpected response\n");
			break;

		default:
			printf("Arduino reports a weird error\n");
	}
}

void handleMessage(TPacket *packet) {
	printf("Message from Alex: %s\n", packet->data);
}

void handlePacket(TPacket *packet) {
	switch(packet->packetType) {
		case PACKET_TYPE_COMMAND:
			// Only we send command packets, so ignore
			break;

		case PACKET_TYPE_RESPONSE:
			handleResponse(packet);
			break;

		case PACKET_TYPE_ERROR:
			handleErrorResponse(packet);
			break;

		case PACKET_TYPE_MESSAGE:
			handleMessage(packet);
			break;
	}
}

void sendPacket(TPacket *packet) {
	char buffer[PACKET_SIZE];
	int len = serialize(buffer, packet, sizeof(TPacket));
	serialWrite(buffer, len);
}

void *receiveThread(void *p) {
	char buffer[PACKET_SIZE];
	int len;
	TPacket packet;
	TResult result;
	int counter=0;

	while(1) {
		len = serialRead(buffer);
		counter+=len;
		if(len > 0) {
			result = deserialize(buffer, len, &packet);

			if(result == PACKET_OK) {
				counter=0;
				handlePacket(&packet);
				
			} else if(result != PACKET_INCOMPLETE) {
				printf("PACKET ERROR\n");
				handleError(result);
			}
		}
	}
}

void flushInput() {
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

void getParams(TPacket *commandPacket) {
	printf("Enter distance/angle in cm/degrees (e.g. 50) and power in %% (e.g. 75) separated by space.\n");
	printf("E.g. 50 75 means go at 50 cm at 75%% power for forward/backward, or 50 degrees left or right turn at 75%%  power\n");
	scanf("%d %d", &commandPacket->params[0], &commandPacket->params[1]);
	flushInput();
}

void sendCommand(char command, bool manual) {
	TPacket commandPacket;

	commandPacket.packetType = PACKET_TYPE_COMMAND;
	command = tolower(command);
	
	printf("\n");
	switch(command) {
		case FORWARD:
			printf("FORWARD\n");
			if (manual) getParams(&commandPacket);
			commandPacket.command = COMMAND_FORWARD;
			sendPacket(&commandPacket);
			break;

		case REVERSE:
			printf("REVERSE\n");
			if (manual) getParams(&commandPacket);
			commandPacket.command = COMMAND_REVERSE;
			sendPacket(&commandPacket);
			break;

		case LEFT:
			printf("LEFT\n");
			if (manual) getParams(&commandPacket);
			commandPacket.command = COMMAND_TURN_LEFT;
			sendPacket(&commandPacket);
			break;

		case RIGHT:
			printf("RIGHT\n");
			if (manual) getParams(&commandPacket);
			commandPacket.command = COMMAND_TURN_RIGHT;
			sendPacket(&commandPacket);
			break;

		case STOP:
			printf("STOP\n");
			commandPacket.command = COMMAND_STOP;
			sendPacket(&commandPacket);
			break;

		case CLEAR:
			printf("CLEAR\n");
			commandPacket.command = COMMAND_CLEAR_STATS;
			// commandPacket.params[0] = 0;
			sendPacket(&commandPacket);
			break;

		case STATS:
			printf("STATS\n");
			commandPacket.command = COMMAND_GET_STATS;
			sendPacket(&commandPacket);
			break;
			
		case SPEED_ONE:
			printf("PARK MODE\n");
			commandPacket.command = COMMAND_SPEED_SLOW;
			sendPacket(&commandPacket);
			break;
			
		case SPEED_TWO:
			printf("NORMAL MODE\n");
			commandPacket.command = COMMAND_SPEED_MID;
			sendPacket(&commandPacket);
			break;
			
		case SPEED_THREE:
			printf("HUMP MODE\n");
			commandPacket.command = COMMAND_SPEED_FAST;
			sendPacket(&commandPacket);
			break;

		case QUIT:
			printf("QUIT\n");
			exitFlag=1;
			break;
			
		case MANUAL:
			if (manual) printf("SET TO MANUAL MODE\n");
			else printf("SET TO AUTO MODE\n");
			
			commandPacket.command = COMMAND_MANUAL;
			sendPacket(&commandPacket);
			break;
			
		case COLOR:
			printf("GET COLOR\n");
			commandPacket.command = COMMAND_COLOR;
			sendPacket(&commandPacket);
			break;
			
		case DISTANCE:
			printf("GET DISTANCE\n");
			commandPacket.command = COMMAND_DIST;
			sendPacket(&commandPacket);
			break;
			
		default:
			send_status = false; // Set status back to idle
			printf("Bad command\n");
	}
}

int main() {
	// Connect to the Arduino
	startSerial(PORT_NAME, BAUD_RATE, 8, 'N', 1, 5);

	// Sleep for two seconds
	printf("WAITING TWO SECONDS FOR ARDUINO TO REBOOT\n");
	sleep(2);
	printf("DONE\n");

	// Spawn receiver thread
	pthread_t recv;

	pthread_create(&recv, NULL, receiveThread, NULL);

	// Send a hello packet
	TPacket helloPacket;

	helloPacket.packetType = PACKET_TYPE_HELLO;
	sendPacket(&helloPacket);
	
	int manual = false;
	printf("\nWASD for movement, f=stop, e=get stats, r=clear stats, q=exit\n");
	while(!exitFlag) {
		char ch;
		
		if (manual) {
			printf("\nWASD for movement, f=stop, e=get stats, r=clear stats, q=exit\n");
			scanf("%c", &ch);
			flushInput(); // Purge extraneous characters from input stream
		} else ch = getch(); // Auto mode
		
		manual = ch == MANUAL ? !manual : manual; // Only toggle when 'm' clicked
		if (!send_status) {
			send_status = true;
			sendCommand(ch, manual);
		}
	}

	printf("Closing connection to Arduino.\n");
	endSerial();
}
