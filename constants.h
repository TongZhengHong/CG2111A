#ifndef __CONSTANTS_INC__
#define __CONSTANTS_INC__

/* 
 *  This file containts all the packet types, commands
 *  and status constants
 *  
 */
 
const char FORWARD = 'w';
const char REVERSE = 's';
const char LEFT = 'a';
const char RIGHT = 'd';
const char STOP = 'f';
const char STATS = 'e';
const char CLEAR = 'r';
const char QUIT = 'q';
const char MANUAL = 'm';
const char COLOR = 'c';
const char DISTANCE = 'v';
const char PARK_MODE = '1';
const char NORMAL_MODE = '2';
const char HUMP_MODE = '3';

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
  COMMAND_PARK_MODE = 7,
  COMMAND_NORMAL_MODE = 8,
  COMMAND_HUMP_MODE = 9,
  COMMAND_MANUAL = 10,
  COMMAND_COLOR = 11,
  COMMAND_DIST = 12
} TCommandType;
#endif

