#ifndef __CONSTANTS_INC__
#define __CONSTANTS_INC__

/* 
 *  This file containts all the packet types, commands
 *  and status constants
 *  
 */

// Motor speed constants
const float SPEED_SLOW = 60.0;
const float SPEED_MID = 80.0;
const float SPEED_FAST = 100.0;

// Distance constants
const float DIST_SHORT = 2.0;
const float DIST_MID = 5.0;
const float DIST_FAR = 25.0;

// Angle constants
const float ANG_SHORT = 50.0;
const float ANG_MID = 100.0;
const float ANG_FAR = 250.0;

// Alex directions
typedef enum {
  STOP = 0,
  FORWARD = 1,
  REVERSE = 2,
  LEFT = 3,
  RIGHT = 4
} TDirection;

// Packet types
typedef enum
{
  PACKET_TYPE_COMMAND = 0,
  PACKET_TYPE_RESPONSE = 1,
  PACKET_TYPE_ERROR = 2,
  PACKET_TYPE_MESSAGE = 3,
  PACKET_TYPE_HELLO = 4
} TPacketType;

// Response types. This goes into the command field
typedef enum
{
  RESP_OK = 0,
  RESP_STATUS=1,
  RESP_BAD_PACKET = 2,
  RESP_BAD_CHECKSUM = 3,
  RESP_BAD_COMMAND = 4,
  RESP_BAD_RESPONSE = 5,
  RESP_COLOR = 6,
  RESP_DIST = 7
} TResponseType;


// Commands
// For direction commands, param[0] = distance in cm to move
// param[1] = speed
typedef enum
{
  COMMAND_FORWARD = 0,
  COMMAND_REVERSE = 1,
  COMMAND_TURN_LEFT = 2,
  COMMAND_TURN_RIGHT = 3,
  COMMAND_STOP = 4,
  COMMAND_GET_STATS = 5,
  COMMAND_CLEAR_STATS = 6,
  COMMAND_SPEED_SLOW = 7,
  COMMAND_SPEED_MID = 8,
  COMMAND_SPEED_FAST = 9,
  COMMAND_MANUAL = 10,
  COMMAND_COLOR = 11,
  COMMAND_DIST = 12
} TCommandType;
#endif
