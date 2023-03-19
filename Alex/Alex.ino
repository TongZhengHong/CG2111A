#include <serialize.h>
#include <stdarg.h>

#include "packet.h"
#include "constants.h"

volatile TDirection dir = STOP;
volatile TMotorSpeed speed = SPEED_SLOW;

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
#define LF                  6   // Left forward pin
#define LR                  5   // Left reverse pin
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

//unsigned long targetDist;
//unsigned long targetTurnTicks;

/*
   Alex Communication Routines.
*/

TResult readPacket(TPacket *packet) {
  // Reads in data from the serial port and
  // deserializes it.Returns deserialized
  // data in "packet".

  char buffer[PACKET_SIZE];
  int len;

  len = readSerial(buffer);

  if (len == 0)
    return PACKET_INCOMPLETE;
  else
    return deserialize(buffer, len, packet);

}

void sendStatus() {
  // Implement code to send back a packet containing key
  // information like leftTicks, rightTicks, leftRevs, rightRevs
  // forwardDist and reverseDist
  // Use the params array to store this information, and set the
  // packetType and command files accordingly, then use sendResponse
  // to send out the packet. See sendMessage on how to use sendResponse.
  //
  TPacket statusPacket;
  statusPacket.packetType = PACKET_TYPE_RESPONSE;
  statusPacket.command = RESP_STATUS;

  statusPacket.params[0] = leftForwardTicks;
  statusPacket.params[1] = rightForwardTicks;
  statusPacket.params[2] = leftReverseTicks;
  statusPacket.params[3] = rightReverseTicks;

  statusPacket.params[4] = leftForwardTicksTurn;
  statusPacket.params[5] = rightForwardTicksTurn;
  statusPacket.params[6] = leftReverseTicksTurn;
  statusPacket.params[7] = rightReverseTicksTurn;

  statusPacket.params[8] = forwardDist;
  statusPacket.params[9] = reverseDist;

  sendResponse(&statusPacket);
}

void sendMessage(const char *message) {
  // Sends text messages back to the Pi. Useful
  // for debugging.

  TPacket messagePacket;
  messagePacket.packetType = PACKET_TYPE_MESSAGE;
  strncpy(messagePacket.data, message, MAX_STR_LEN);
  sendResponse(&messagePacket);
}

void dbprintf(char *format, ...) {
  va_list args;
  char buffer[128];
  va_start(args, format);
  vsprintf(buffer, format, args);
  sendMessage(buffer);
}

void sendBadPacket() {
  // Tell the Pi that it sent us a packet with a bad
  // magic number.

  TPacket badPacket;
  badPacket.packetType = PACKET_TYPE_ERROR;
  badPacket.command = RESP_BAD_PACKET;
  sendResponse(&badPacket);
}

void sendBadChecksum() {
  // Tell the Pi that it sent us a packet with a bad
  // checksum.

  TPacket badChecksum;
  badChecksum.packetType = PACKET_TYPE_ERROR;
  badChecksum.command = RESP_BAD_CHECKSUM;
  sendResponse(&badChecksum);
}

void sendBadCommand() {
  // Tell the Pi that we don't understand its
  // command sent to us.

  TPacket badCommand;
  badCommand.packetType = PACKET_TYPE_ERROR;
  badCommand.command = RESP_BAD_COMMAND;
  sendResponse(&badCommand);
}

void sendBadResponse() {
  TPacket badResponse;
  badResponse.packetType = PACKET_TYPE_ERROR;
  badResponse.command = RESP_BAD_RESPONSE;
  sendResponse(&badResponse);
}

void sendOK() {
  TPacket okPacket;
  okPacket.packetType = PACKET_TYPE_RESPONSE;
  okPacket.command = RESP_OK;
  sendResponse(&okPacket);
}

void sendResponse(TPacket *packet) {
  // Takes a packet, serializes it then sends it out
  // over the serial port.
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}

/*
   Setup and start codes for serial communications
*/
// Set up the serial connection. For now we are using
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial() {
  // To replace later with bare-metal.
  Serial.begin(9600);
}

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.

void startSerial() {
  // Empty for now. To be replaced with bare-metal code
  // later on.

}

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid.
// This will be replaced later with bare-metal code.

int readSerial(char *buffer) {

  int count = 0;

  while (Serial.available())
    buffer[count++] = Serial.read();

  return count;
}

// Write to the serial port. Replaced later with
// bare-metal code

void writeSerial(const char *buffer, int len) {
  Serial.write(buffer, len);
}

/*
   Alex's setup and run codes
*/

// Clears all our counters
void clearCounters() {
  leftForwardTicks = 0;
  rightForwardTicks = 0;
  leftReverseTicks = 0;
  rightReverseTicks = 0;

  leftForwardTicksTurn = 0;
  rightForwardTicksTurn = 0;
  leftReverseTicksTurn = 0;
  rightReverseTicksTurn = 0;

  leftRevs = 0;
  rightRevs = 0;

  forwardDist = 0;
  reverseDist = 0;
}

// Clears one particular counter
void clearOneCounter(int which) {
  clearCounters(); return;

  switch (which) {
    case 0:
      clearCounters();
      break;

    case 1:
      leftForwardTicks = 0;
      break;

    case 2:
      rightForwardTicks = 0;
      break;

    case 3:
      leftRevs = 0;
      break;

    case 4:
      rightRevs = 0;
      break;

    case 5:
      forwardDist = 0;
      break;

    case 6:
      reverseDist = 0;
      break;
  }
}

// Intialize Vincet's internal states
void initializeState() {
  clearCounters();
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

    case COMMAND_SPEED_SLOW:
      sendOK();
      speed = SPEED_SLOW;
      break;

    case COMMAND_SPEED_MID:
      sendOK();
      speed = SPEED_MID;
      break;

    case COMMAND_SPEED_FAST:
      sendOK();
      speed = SPEED_FAST;
      break;

    case COMMAND_GET_STATS:
      sendOK();
      sendStatus();
      break;

    case COMMAND_CLEAR_STATS:
      sendOK();
      clearOneCounter(command->params[0]);
      break;

    default:
      sendBadCommand();
  }
}

void waitForHello() {
  int exit = 0;

  while (!exit) {
    TPacket hello;
    TResult result;

    do {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if (result == PACKET_OK) {
      if (hello.packetType == PACKET_TYPE_HELLO) {
        sendOK();
        exit = 1;
      } else sendBadResponse();
    } else if (result == PACKET_BAD) {
      sendBadPacket();
    } else if (result == PACKET_CHECKSUM_BAD)
      sendBadChecksum();
  } // !exit
}

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

  else if (result == PACKET_BAD) {
    sendBadPacket();

  } else if (result == PACKET_CHECKSUM_BAD) {
    sendBadChecksum();
  }
}
