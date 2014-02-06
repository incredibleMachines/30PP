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
    
    //setup and update
    SocketHandler();
    void setup(int port, bool verboseMode);
    void update();

    //init events
    void sendInitCmd();
    void initEvents(ofxLibwebsockets::Event &args);
    bool eventsInited;
    
    //socket client
    ofxLibwebsockets::Client socketClient;
    ofxLibwebsockets::ClientOptions socketOptions;
    void connectSocket();
    bool socketConnected;
    bool reconnectFlag;
    
    //socket callbacks
    void onConnect(ofxLibwebsockets::Event& args);
    void onOpen(ofxLibwebsockets::Event& args);
    void onClose(ofxLibwebsockets::Event& args);
    void onIdle(ofxLibwebsockets::Event& args);
    void onMessage(ofxLibwebsockets::Event& args);
    void onBroadcast(ofxLibwebsockets::Event& args);
    
    //socket debug
    void drawDebugInfo();
    string debugActivityInfo;
    bool bVerbose;
    

};
