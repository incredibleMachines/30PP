//
//  socketHandler.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/5/14.
//
//
/*
 
 SocketHandler class. handles all socket communications with controllerApp. Also handles all of the Event Init request and kickoff.
 
 methods
    - setup takes the port argument and instantiates the websocket Client on that port
    - update checks the connection status and attempts to connect if needed. if connected, requests init, sends heartbeats, updates
    - drawDebugInfo draws the connection status to screen
    - sendInitCmd sends init command to controllerApp, requesting init message of all events
    - initEvents creates all Events, Scenes, Assets, Files, sets eventsInited to true
 
 */

#include "SocketHandler.h"
#include "playerApp.h"

//--------------------------------------------------------------
SocketHandler::SocketHandler(){
 
    reconnectFlag = false;
}

//--------------------------------------------------------------
void SocketHandler::setup(int port, bool verboseMode){
    
    socketOptions = ofxLibwebsockets::defaultClientOptions();
    socketOptions.port = port;
    socketConnected = socketClient.connect(socketOptions);
    socketClient.addListener(this);
    
    bVerbose = verboseMode;
}

//--------------------------------------------------------------
void SocketHandler::update(){
    
    bool sec = ofGetUnixTime() % 2; //which second are we on
    
    
    //--- we're NOT connected to controllerApp ! let's try to connect
    if(!socketConnected){
        if(sec && reconnectFlag){  // attempt reconnect once every 2 secs
            
            //--- attempt to Client.connect()
            connectSocket();
            
            reconnectFlag = 0;
        } else if (!sec) reconnectFlag = 1;
    }
    
    
    //--- socket IS connected
    else {
        
        //--- events NOT inited yet, let's try to init
        if (!eventHandler.eventsInited){
            if(sec && reconnectFlag){ // attempt request init cmd once every 2 secs

                //--- send INIT_REQUEST to controller app
                sendSocketCmd(INIT_REQ);
                
                reconnectFlag = 0;
            } else if (!sec) reconnectFlag = 1;
        }
        
        
        //--- we are connected AND all eventsInited
        else {
            if(sec && reconnectFlag){
                
                //TODO: status update / heartbeat can go here
                
                reconnectFlag = 0;
            } else if (!sec) reconnectFlag = 1;
        }
    }
}

//--------------------------------------------------------------
void SocketHandler::connectSocket(){
    
    cout << "-------------------------" << endl;
    socketConnected = socketClient.connect(socketOptions);
    
    debugActivityInfo = "ATTEMPTED connectSocket() || "+ofGetTimestampString();
    cout << "Connection Status: " << socketConnected << endl;
    cout << ">>> socketHandler: "<< debugActivityInfo << endl << endl;
}


//--------------------------------------------------------------
void SocketHandler::sendSocketCmd(string cmd){ //TODO: turn into generic sendMsg(string msg)
    
    cout << "-----------------------" << endl;
    socketClient.send(cmd);
    
    debugActivityInfo = "SENT "+cmd+" || Time: "+ofGetTimestampString();
    cout << ">>> socketHandler:  "<< debugActivityInfo << endl << endl;
}



//--------------------------------------------------------------
void SocketHandler::onMessage(ofxLibwebsockets::Event &args){
    
    cout <<"\n==================== SOCKET MESSAGE RECEIVED ====================="<<endl;
    string thisCmd = args.json["command"].asString();
    cout <<">>> raw message:\n" << args.message << endl;
    cout <<"======================== END SOCKET MESSAGE ========================"<<endl;
    cout <<"\n>>> thisCmd = "<< thisCmd << endl;
    eventHandler.processEvent(thisCmd, args);

}

//--------------------------------------------------------------
void SocketHandler::drawDebugInfo(){
    
    //--- draw some info
    ofBackground(0);
    
    if (!socketConnected){ // socket not connected !
        ofSetColor(255,0,20);
        ofDrawBitmapString("NOT CONNECTED", 140, ofGetHeight()-50);
        
    } else {  // socket connected
        //--- connected msg
        ofSetColor(0,255,20);
        ofDrawBitmapString("CONNECTED", 140, ofGetHeight()-50);
        if(eventHandler.eventsInited){
            ofDrawBitmapString("+ EVENTS INITED", 220, ofGetHeight()-50);
        } else {
            ofSetColor(255, 0, 20);
            ofDrawBitmapString("+ EVENTS NOT INITED", 220, ofGetHeight()-50);
        }
        
    }
    
    ofSetColor(255);
    ofDrawBitmapString("socket status: ", 20, ofGetHeight()-50);
    if(bVerbose){
        ofDrawBitmapString("'i' to request init manually", 20, ofGetHeight()-30);
        //ofSetColor(200, 200, 200);
        ofDrawBitmapString(debugActivityInfo, 20, ofGetHeight()-10);
    }
}



//--------------------------------------------------------------
void SocketHandler::onConnect(ofxLibwebsockets::Event &args){
    cout << "Websocket onConnect" << endl;
    
}

//--------------------------------------------------------------
void SocketHandler::onClose(ofxLibwebsockets::Event &args){
    cout << "Websocket onClose"<<endl;
    socketConnected = false;
    
}

//--------------------------------------------------------------
void SocketHandler::onOpen(ofxLibwebsockets::Event &args){
    cout << "Websocket onOpen" << endl;
    
}

//--------------------------------------------------------------
void SocketHandler::onBroadcast(ofxLibwebsockets::Event &args){
    cout << "Websocket onBroadcast"<< args.message << endl;
    
}

//--------------------------------------------------------------
void SocketHandler::onIdle(ofxLibwebsockets::Event &args){
    cout << "Websocket onIdle" << endl;
    
}

