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

//constants
#define INIT_CMD "{\"command\" : \"init\"}"
#define STATUS_MSG "{\"command\" : \"statusinfoTBD\"}"
#define HEARTBEAT_MSG "{\"command\" : \"heartbeat\"}"

//--------------------------------------------------------------
SocketHandler::SocketHandler(){
 
    reconnectFlag = true;
    eventsInited = false;
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
        if (!((playerApp*)ofGetAppPtr())->eventsInited){
            if(sec && reconnectFlag){ // attempt request init cmd once every 2 secs

                //--- send INIT_CMD to controller app
                sendInitCmd();
                
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
void SocketHandler::sendInitCmd(){ //TODO: turn into generic sendMsg(string msg)
    
    cout << "-----------------------" << endl;
    socketClient.send(INIT_CMD);
    
    debugActivityInfo = "SENT INIT_CMD || Time: "+ofGetTimestampString();
    cout << ">>> socketHandler:  "<< debugActivityInfo << endl << endl;
}


//--------------------------------------------------------------
void SocketHandler::initEvents(ofxLibwebsockets::Event &args){
    
    /* kicks off Event, Scene, and Asset vector filling */
    
    int numEvents = args.json["events"].size();
    
    cout<<">>> hit initEvents()"<<endl;
    cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>--------------START EVENT INIT--------------<<"<<endl<<endl;
    
    for (int e=0; e<numEvents; e++) {
        Event thisEvent = * new Event (args.json["events"].get(e, "1 complete event"));
        cout<<endl;
        ((playerApp*)ofGetAppPtr())->allEvents.push_back(thisEvent);
    }
    
    cout<<"\t>>--------------END EVENT INIT--------------<<"<<endl;
    
    // set eventsInited true
    ((playerApp*)ofGetAppPtr())->eventsInited = true;
}

//--------------------------------------------------------------
void SocketHandler::drawDebugInfo(){
    //--- draw some info
    ofBackground(0);
    
    if (!socketConnected){ // socket not connected !
        ofSetColor(255,0,20);
        ofDrawBitmapString("NOT CONNECTED", 140, 32);
        
    } else {  // socket connected
        //--- connected msg
        ofSetColor(0,255,20);
        ofDrawBitmapString("CONNECTED", 140, 32);
        if(((playerApp*)ofGetAppPtr())->eventsInited){
            ofDrawBitmapString("+ EVENTS INITED", 220, 32);
        } else {
            ofSetColor(255, 0, 20);
            ofDrawBitmapString("+ EVENTS NOT INITED", 220, 32);
        }

    }
    
    ofSetColor(255);
    ofDrawBitmapString("socket status: ", 20, 30);
    if(bVerbose){
        ofDrawBitmapString("'i' to request init manually", 20, ofGetHeight()-20);
        ofSetColor(200, 200, 200);
        ofDrawBitmapString(debugActivityInfo, 20, 50);
    }
}




//--------------------------------------------------------------
void SocketHandler::onMessage(ofxLibwebsockets::Event &args){
    
    cout <<"\n==================== SOCKET MESSAGE RECEIVED ====================="<<endl;
    string thisCmd = args.json["command"].asString();
    cout <<">>> raw message:\n" << args.message << endl;
    cout <<"======================== END SOCKET MESSAGE ========================"<<endl;
    cout <<"\n>>> thisCmd = "<< thisCmd << endl;
    
    //TODO: make switch cases
    
    //--- init assets
    if (thisCmd == "init"){
        cout << ">>> received init" << endl;
        initEvents(args);
    }
    
    //--- start an event
    else if (thisCmd == "start"){
        cout << ">>> received start event" << endl;
        //cout<<"EVENT NUMBER: "<< allEvents.size() << endl;
        //startEvent(args); <-- TODO
    }
    
    //--- stop an event
    else if (thisCmd == "stop"){
        cout << ">>> received stop event" << endl;
        //stopEvent(args); <-- TODO
    }
    
    //--- server error
    else if (thisCmd == "error"){
        cout << ">>> received server error" << endl;
        // server has returned an error
    }
    
    //--- unknown event
    else {
        cout << ">>> received UNKNOWN EVENT" << endl;
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

