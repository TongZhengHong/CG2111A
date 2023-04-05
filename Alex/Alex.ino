//#include "Arduino.h"
//#include <avr/io.h>

#include <serialize.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "packet.h"
#include "constants.h"
#define strtof(A, B) strtod(A, B)

//volatile TDirection dir = STOP;
//volatile TMotorSpeed speed = SPEED_SLOW;
volatile float speed = PRESET_SPEED;
volatile float dist = PRESET_DIST;
volatile long ang = 0;
volatile char dir = PRESET_DIR;

TTokenType tokenStatuses[4] = {UNCHECKED_TOKEN};

/*
   Alex's configuration constants
 */

// Number of ticks per revolution from the
// wheel encoder.

#define COUNTS_PER_REV      170

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          20.42 // 3.1415 * 6.5cm 
#define ALEX_CIRC           56.55 // Alex circumference in cm = 3.1415 * 18cm (diameter)

// Motor control pins. You need to adjust these till
// Alex moves in the correct direction
#define LF                  5   // Left forward pin
#define LR                  6   // Left reverse pin
#define RF                  10  // Right forward pin
#define RR                  11  // Right reverse pin

/*
   Alex's State Variables
 */

// Store the ticks from Alex's left and right encoders.
volatile unsigned long leftForwardTicks;
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks;
volatile unsigned long rightReverseTicks;

// Left and right ticks for turning
volatile unsigned long leftForwardTicksTurn;
volatile unsigned long rightForwardTicksTurn;
volatile unsigned long leftReverseTicksTurn;
volatile unsigned long rightReverseTicksTurn;

// Store the revolutions on Alex's left and right wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

// Forward and backward distance travelled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;


void setup() {
	cli();
	setupEINT();
	setupSerial();
	startSerial();

	setupMotors();
	startMotors();

	enablePullups();
	initializeState();
	sei();
}

TTokenType checkSpeedToken(char *userStr)
{
	char *junkStr;
	if (strtol(userStr, &junkStr, 10) < 0 || strtol(userStr, &junkStr, 10) > 100)
	{
		sendBadToken(SPEED_TOKEN_BAD);
		sendMessage("Speed received by Arduino is: %f\n",strtof(userStr, &junkStr));
		return SPEED_TOKEN_BAD;
	}
	else {
		return TOKEN_GOOD;
	}
}
void processTokens(char *userStr)
{
	int badTokens = 0;
	for (int i = 0; i < 4; i++)
	{
		if (tokenStatuses[i])
		{
			badTokens++;
		}
	}
	if (badTokens)
	{
		sendBadToken(tokenStatuses[0]);
		sendBadToken(tokenStatuses[1]);
		sendBadToken(tokenStatuses[2]);
		sendBadToken(tokenStatuses[3]);
		sendTokenData(speed,dist,ang,dir);
	}
	else
	{
		char *junkStr;
		const char delim[2] = " ";
		char *curToken = strtok(userStr, delim);
		speed = strtof(curToken, &junkStr);
		curToken = strtok(NULL, delim);
		dist = strtof(curToken, &junkStr);
		curToken = strtok(NULL, delim);
		ang = strtol(curToken, &junkStr, 10);
		curToken = strtok(NULL, delim);
		dir = *curToken;
		sendTokenData(speed,dist,ang,dir);
	}
}
void checkTokens(char *userStr)
{
	char *junkStr;
	const char delim[2] = " ";
	long checkTokens[3];
	//0 for checkSpeed, 1 for checkDist, 2 for checkAng
	char *curToken = strtok(userStr, delim); 
	for (int i = 0; i < 3; i++)
	{
		checkTokens[i] = strtol(curToken, &junkStr, 10);
		if (i == 0) {
			if (checkTokens[i] < 0 || checkTokens[i] > 100)
			{
				tokenStatuses[i] = SPEED_TOKEN_BAD;
			}
			else {
				tokenStatuses[i] = TOKEN_GOOD;
			}
		}
		else if (i == 1) {
			if (checkTokens[i] < DIST_MIN || checkTokens[i] > DIST_MAX) {
				tokenStatuses[i] = DIST_TOKEN_BAD; 
			}
			else {
				tokenStatuses[i] = TOKEN_GOOD;
			}
		}
		else if (i == 2) {
			if (checkTokens[i] < 0 || checkTokens[i] > 360) {
				tokenStatuses[i] = ANG_TOKEN_BAD;
			}
			else {
				tokenStatuses[i] = TOKEN_GOOD;
			}
		}

		curToken = strtok(NULL, delim);
	}
	char checkDir = *curToken;
	if (checkDir != 'w' || checkDir != 'a' || checkDir != 's' || checkDir != 'd')
	{
		tokenStatuses[3] = DIR_TOKEN_BAD;
	}
}

void handlePacket(TPacket *packet) {
	switch (packet->packetType) {
		case PACKET_TYPE_COMMAND:
			handleCommand(packet);
			break;

		case PACKET_TYPE_RESPONSE:
			break;

		case PACKET_TYPE_ERROR:
			break;

		case PACKET_TYPE_MESSAGE:
			break;

		case PACKET_TYPE_HELLO:
			break;
	}
}
void handleCommand(TPacket *command) {
	switch (command->command) {
		// For movement commands, param[0] = distance, param[1] = speed.
		case COMMAND_FORWARD:
			sendOK();
			forward(); //(float) command->params[0], (float) command->params[1]
			break;

		case COMMAND_REVERSE:
			sendOK();
			reverse();
			break;

		case COMMAND_TURN_LEFT:
			sendOK();
			left();
			break;

		case COMMAND_TURN_RIGHT:
			sendOK();
			right();
			break;

		case COMMAND_STOP:
			sendOK();
			stop();
			break;

		case COMMAND_GET_STATS:
			sendOK();
			sendStatus();
			break;

		case COMMAND_CLEAR_STATS:
			sendOK();
			clearOneCounter(command->params[0]);
			break;
			//Speed config is meant for auto
		case COMMAND_SPEED_CONFIG:
			sendOK();
			char *junkChar;
			if (checkSpeedToken(command->data))
			{
				sendBadToken(checkSpeedToken(command->data));
				break;
			}
			speed = strtof(command->data, &junkChar);
			break;
		case COMMAND_MANUAL:
			sendOK();
			checkTokens(command->data);
			processTokens(command->data);
			manualMove(speed, dist, ang, dir);
			break;

		default:
			sendBadCommand();
	}
}

void loop() {
	// Uncomment the code below for Step 2 of Activity 3 in Week 8 Studio 2

	//  forward(0, 30);
	//  float pi = 3.141592654;
	//  dbprintf("PI is %3.2f\n", pi);

	//  if (dir == FORWARD) {
	//    if (forwardDist >= targetDist) {
	//      targetDist = 0;
	//      stop();
	//    }
	//  } else if (dir == REVERSE) {
	//    if (reverseDist >= targetDist) {
	//      targetDist = 0;
	//      stop();
	//    }
	//  } else if (dir == RIGHT) {
	//    if (rightReverseTicks >= targetTurnTicks) {
	//      targetTurnTicks = 0;
	//      stop();
	//    }
	//  } else if (dir == LEFT) {
	//    if (leftReverseTicks >= targetTurnTicks) {
	//      targetTurnTicks = 0;
	//      stop();
	//    }
	//  }

	TPacket recvPacket; // This holds commands from the Pi
	TResult result = readPacket(&recvPacket);

	if (result == PACKET_OK)
		handlePacket(&recvPacket);
	else if (result == PACKET_BAD)
		sendBadPacket();
	else if (result == PACKET_CHECKSUM_BAD)
		sendBadChecksum();
	tokenStatuses[3] = {UNCHECKED_TOKEN};
}
