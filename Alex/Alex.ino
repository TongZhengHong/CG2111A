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
#define RF                  11  // Right forward pin
#define RR                  3  // Right reverse pin

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
  else if (result == PACKET_BAD) 
    sendBadPacket();
  else if (result == PACKET_CHECKSUM_BAD) 
    sendBadChecksum();
}
