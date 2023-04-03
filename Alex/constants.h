#ifndef __CONSTANTS_INC__
#define __CONSTANTS_INC__

/* 
 *  This file containts all the packet types, commands
 *  and status constants
 *  
 */

// Motor speeds
typedef enum {
  SPEED_SLOW = 60,
  SPEED_MID = 80,
  SPEED_FAST = 100
} TMotorSpeed;

// Distances
typedef enum {
  DIST_SHORT = 2,
  DIST_MID = 5,
  DIST_FAR = 20
} TDistances;

typedef enum {
  ANG_SHORT = 5,
  ANG_MID = 8,
  ANG_FAR = 11
} TAngles;

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
  RESP_BAD_RESPONSE = 5 
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
  COMMAND_MANUAL = 10
} TCommandType;
#endif

