//
//  Definitions.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/7/14.
//
//
/* Definitions.h
 
 Created this for constant strings, vars, etc
 - socket server commands and messages
*/

#pragma once

#include "ofMain.h"


//--- SOCKET SERVER REQUESTS
#define INIT_REQ    "{\"command\" : \"init\"}"
#define GO_REQ      "{\"command\" : \"go\"}"


//--- SOCKET SERVER COMMANDS
#define INIT_CMD    "init"
#define SALES_CMD   "sales"
#define INTRO_CMD   "intro"
#define OUTRO_CMD   "outro"
#define ERROR_CMD   "error"


//--- SOCKET SERVER MESSAGES
#define STATUS_MSG "{\"command\" : \"statusinfoTBD\"}"
#define HEARTBEAT_MSG "{\"command\" : \"heartbeat\"}"