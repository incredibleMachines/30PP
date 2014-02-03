//
//  Event.h
//  PlayerApp
//  Created by Joseph Saavedra on 1/2/14.
/*
 
 Event Object

 gets instantiated on each incoming Event object, all fields + associated Asset object fields are populated,
 (TODO: load assets)
 
*/
//

#pragma once
#include "ofMain.h"
#include "Scene.h"
#include "ofxLibwebsockets.h"

// Event class
class Event {
public:
    
//    Event(ofxLibwebsockets::Event json);
    Event(Json::Value thisEvent);
    //void initEvent(ofxLibwebsockets::Event &args);
    
    string eCommand;
    string eTitle;
    string eType;
    string eAnimType;
    string eId;
    string eCreatedAt;
    
    vector <Scene> eScenes; //vector of all scenes for this event
    
private:
    
};

