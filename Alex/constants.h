#ifndef __CONSTANTS_INC__
#define __CONSTANTS_INC__

/* 
 *  This file containts all the packet types, commands
 *  and status constants
 *  
 */

// Motor speeds
/*typedef enum {
  SPEED_SLOW = 60,
  SPEED_MID = 80,
  SPEED_FAST = 100
} TMotorSpeed*/;

// Alex directions
/*typedef enum {
  STOP = 0,
  FORWARD = 1,
  REVERSE = 2,
  LEFT = 3,
  RIGHT = 4
} TDirection;*/

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
  RESP_BAD_TOKEN = 6;
} TResponseType;

typedef enum
{
	TOKEN_GOOD = 0,
	SPEED_TOKEN_BAD = 1,
	DIST_TOKEN_BAD = 2,
	DIR_TOKEN_BAD = 3
} TTokenType;
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
  COMMAND_SPEED_CONFIG = 7,
  COMMAND_MANUAL = 8,
} TCommandType;
#endif

