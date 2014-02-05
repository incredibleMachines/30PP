//
//  socketHandler.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/5/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include "Event.h"

class SocketHandler{

public:
    
    SocketHandler();
    void setup(int port);
    void update();
    void drawDebugInfo();
    void sendInitCmd();
    void initEvents(ofxLibwebsockets::Event &args);
    
    ofxLibwebsockets::Client socketClient;
    ofxLibwebsockets::ClientOptions socketOptions;
    bool socketConnected;
    
    void onConnect(ofxLibwebsockets::Event& args);
    void onOpen(ofxLibwebsockets::Event& args);
    void onClose(ofxLibwebsockets::Event& args);
    void onIdle(ofxLibwebsockets::Event& args);
    void onMessage(ofxLibwebsockets::Event& args);
    void onBroadcast(ofxLibwebsockets::Event& args);
    
    bool eventsInited;
};
