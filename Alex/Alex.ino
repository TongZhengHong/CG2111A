//#include "Arduino.h" 
//#include <avr/io.h> 

#include <serialize.h>
#include <stdarg.h>

#include "packet.h"
#include "constants.h"

bool MANUAL_MODE = false;

volatile TDirection dir = STOP;
volatile int speed = SPEED_FAST;

volatile int distance = DIST_MID;
unsigned long targetDist;

volatile int angle = ANG_MID;
unsigned long targetTurnTicks;

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
      Alex's Colour Sensor
*/
#define S1 8
#define S0 9
#define S2 12
#define S3 13
#define sensorOut 4

// Frequency read by the photodiodes
int redFreq = 0;
int greenFreq = 0;

// Stores the red and green colour values
int redColour = 0;
int greenColour = 0;

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
  setupColor();

  enablePullups();
  initializeState();
  sei();
}

void handleCommand(TPacket *command) {
  switch (command->command) {
    // For movement commands, param[0] = distance, param[1] = speed.
    case COMMAND_FORWARD:
      sendOK();
      if (MANUAL_MODE) 
        distance = (float) command->params[1];
      forward(); //(float) command->params[0], (float) command->params[1]
      break;

    case COMMAND_REVERSE:
      sendOK();
      if (MANUAL_MODE) 
        distance = (float) command->params[1];
      reverse();
      break;

    case COMMAND_TURN_LEFT:
      sendOK();
      if (MANUAL_MODE) 
        angle = (float) command->params[1];
      left();
      break;

    case COMMAND_TURN_RIGHT:
      sendOK();
      if (MANUAL_MODE) 
        angle = (float) command->params[1];
      right();
      break;

    case COMMAND_STOP:
      sendOK();
      stop();
      break;

    case COMMAND_SPEED_SLOW:
      sendOK();
      distance = DIST_SHORT;
      angle = ANG_SHORT;
      break;

    case COMMAND_SPEED_MID:
      sendOK();
      distance = DIST_MID;
      angle = ANG_MID;
      break;

    case COMMAND_SPEED_FAST:
      sendOK();
      distance = DIST_FAR;
      angle = ANG_FAR;
      break;

    case COMMAND_GET_STATS:
      sendOK();
      sendStatus();
      break;

    case COMMAND_CLEAR_STATS:
      sendOK();
      clearOneCounter(command->params[0]);
      break;

    case COMMAND_MANUAL:
      sendOK();
      MANUAL_MODE = !MANUAL_MODE; // Toggle manual mode
      if (!MANUAL_MODE) { //Reset distance and angle when toggle back to auto mode
        distance = DIST_MID;
        angle = ANG_MID;
      }
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

    if (dir == FORWARD) {
      if (forwardDist >= targetDist) {
        targetDist = 0;
        stop();
      }
    } else if (dir == REVERSE) {
      if (reverseDist >= targetDist) {
        targetDist = 0;
        stop();
      }
    } else if (dir == RIGHT) {
      if (rightReverseTicks >= targetTurnTicks) {
        targetTurnTicks = 0;
        stop();
      }
    } else if (dir == LEFT) {
      if (leftReverseTicks >= targetTurnTicks) {
        targetTurnTicks = 0;
        stop();
      }
    }

  TPacket recvPacket; // This holds commands from the Pi
  TResult result = readPacket(&recvPacket);

  if (result == PACKET_OK) 
    handlePacket(&recvPacket);
  else if (result == PACKET_BAD) 
    sendBadPacket();
  else if (result == PACKET_CHECKSUM_BAD) 
    sendBadChecksum();
}
