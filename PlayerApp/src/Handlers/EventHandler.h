//
//  EventHandler.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/7/14.
//
//
#pragma once
#include "ofMain.h"
#include "../Events/Event.h"
#include "ofxLibwebsockets.h"
#include "../Compositor/Composite.h"


class EventHandler {
public:
    EventHandler();
    void processEvent(string command, ofxLibwebsockets::Event &args);
    
    void initEvents(ofxLibwebsockets::Event &args);
    void salesEvent(ofxLibwebsockets::Event &args);
    //void fadeInEvent(ofxLibwebsockets::Event &args); //TODO
    //void fadeOutEvent(ofxLibwebsockets::Event &args); //TODO
   
    
    vector <Event> allEvents;
    
    vector <Scene> allScenes;
    
    bool eventsInited;
};