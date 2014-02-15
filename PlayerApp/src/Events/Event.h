//
//  Event.h
//  PlayerApp
//  Created by Joseph Saavedra on 1/2/14.
//

#pragma once
#include "ofMain.h"
#include "Scene.h"
#include "ofxLibwebsockets.h"

// Event class
class Event {
public:
    
    Event(Json::Value thisEvent);
    
    string eCommand;
    string eTitle;
    string eType;
    string eAnimType;
    string eId;
    string eCreatedAt;
    
    vector <Scene> eScenes; //vector of all scenes for this event
    
    int numScenes;
    
private:
    
};

