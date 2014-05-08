//
//  EventTypes.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/7/14.
//
//
/* EventTypes.h
 
 Created this for constant strings, vars, etc
 - socket server commands and messages
 */

#pragma once

#include "ofMain.h"


//--- SOCKET SERVER REQUESTS
#define INIT_REQ            "{\"command\" : \"init\"}"
#define GO_REQ              "{\"command\" : \"go\"}" //not currently used
#define CLOSE_REQ           "{\"command\" : \"close\"}"
#define RESTART_REQ         "{\"command\" : \"restart\"}"


//--- SOCKET SERVER COMMANDS
#define INIT_CMD            "init"
#define PLAY_CMD            "play"
#define SALES_CMD           "sales"
#define AMBIENT_CMD         "ambient"

#define PAUSE_CMD           "pause"
#define RESUME_CMD          "resume"
#define END_CMD             "end"
#define ERROR_CMD           "error"


//--- SOCKET SERVER MESSAGES
#define STATUS_MSG          "{\"command\" : \"statusinfoTBD\"}"
#define HEARTBEAT_MSG       "{\"command\" : \"heartbeat\"}"
