//
//  EventHandler.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/7/14.
//
//
#pragma once
#include "ofMain.h"
#include "ofxLibwebsockets.h"


class EventHandler {
public:
    EventHandler();
    void processEvent(string command, ofxLibwebsockets::Event &args);
    
    //init and play
    void initEvents (ofxLibwebsockets::Event &args);
    void playEvent  (ofxLibwebsockets::Event &args);
    
    //control and error
    void pauseEvent (ofxLibwebsockets::Event &args);
    void resumeEvent(ofxLibwebsockets::Event &args);
    void endEvent   (ofxLibwebsockets::Event &args);
    void errorEvent (ofxLibwebsockets::Event &args);
    
    //vector <Event> allEvents;
    bool eventsInited;
    
    string movieFile;
    
    class event{
    public:
        float startTime;
        float duration;
        string title;
        vector<int> pauseTimes;
    };
    
    vector<event> events;
    
    bool bTriggerEvent;
    string currentEvent;
    float currentStart;
    
};