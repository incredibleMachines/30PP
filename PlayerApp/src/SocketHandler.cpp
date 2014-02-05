//
//  socketHandler.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/5/14.
//
//
/*
 
 socketHandler class. handles all socket communications with controllerApp.
 
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
    
}

//--------------------------------------------------------------
void SocketHandler::setup(int port){
    
    eventsInited = false;
    
    socketOptions = ofxLibwebsockets::defaultClientOptions();
    socketOptions.port = port;
    socketConnected = socketClient.connect(socketOptions);
    socketClient.addListener(this);
}

//--------------------------------------------------------------
void SocketHandler::update(){
    
    if(!socketConnected){ //--- we're not connected to controllerApp !
        if(ofGetElapsedTimeMillis() % 500 <= 20){ // ~every half second try to reconnect
            cout << "-------------------------" << endl;
            cout << "Socket Not Connected. Attempting Reconnect."<<endl;
            cout<<"Time: "<< ofGetUnixTime() << endl;
            socketConnected = socketClient.connect(socketOptions);
            cout << "Connection Status: " << socketConnected << endl;
        }
    }
    else {  //--- socket IS connected
        if (!eventsInited){  // events NOT inited yet
            if(ofGetElapsedTimeMillis() % 1000 <= 20){ // ~every second request init cmd (for now)
                //sendInitCmd();
            }
        }
        else {  // we are connected AND all eventsInited
            if(ofGetElapsedTimeMillis() % 1000 <= 20){
                // status update / heartbeat can go here
            }
        }
    }
}

//--------------------------------------------------------------
void SocketHandler::sendInitCmd(){
    
    cout << "-----------------------" << endl;
    socketClient.send(INIT_CMD);
    cout << ">>> socketHandler: SENT INIT_CMD  \t||\t Time: "<< ofGetUnixTime() << endl << endl;
}

//--------------------------------------------------------------
void SocketHandler::onConnect(ofxLibwebsockets::Event &args){
    cout << "Websocket onConnect" << endl;
    
}

//--------------------------------------------------------------
void SocketHandler::onOpen(ofxLibwebsockets::Event &args){
    cout << "Websocket onOpen" << endl;
    
}

//--------------------------------------------------------------
void SocketHandler::onClose(ofxLibwebsockets::Event &args){
    cout << "Websocket onClose"<<endl;
    socketConnected = false;
    
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
void SocketHandler::onBroadcast(ofxLibwebsockets::Event &args){
    cout << "Websocket onBroadcast"<< args.message << endl;
    
}

//--------------------------------------------------------------
void SocketHandler::onIdle(ofxLibwebsockets::Event &args){
    cout << "Websocket onIdle" << endl;
    
}


//--------------------------------------------------------------
void SocketHandler::initEvents(ofxLibwebsockets::Event &args){ //fills the allAssets vector on init
    
    cout<<">>> hit initEvents()"<<endl;
    int numEvents = args.json["events"].size();
    cout<<">>> num events received: "<< numEvents <<endl<<endl;
    
    cout<<"\t>>--------------START EVENT INIT--------------<<"<<endl<<endl;
    for (int e=0; e<numEvents; e++) {
        Event thisEvent = * new Event (args.json["events"].get(e, "1 complete event"));
        cout<<endl;
        ((playerApp*)ofGetAppPtr())->allEvents.push_back(thisEvent);
    }
    cout<<"\t>>--------------END EVENT INIT--------------<<"<<endl;
    
    //--- syphon setup
    //allSyphons.setup( allEvents ); //pass in ref to allAssets here ( might move to setup )
    
    eventsInited = true; //we're inited, ready to go
}

//--------------------------------------------------------------
void SocketHandler::drawDebugInfo(){
    //--- draw some info
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString("socket status: ", 20, 30);
    ofDrawBitmapString("'i' to request init manually", 20, ofGetHeight()-20);
    ofDrawBitmapString("socket status: ", 20, 30);

    if (!socketConnected){ // socket not connected !
        ofSetColor(255,0,20);
        ofDrawBitmapString("NOT CONNECTED", 140, 32);
        
    } else {  // socket connected
        //--- connected msg
        ofSetColor(0,255,20);
        ofDrawBitmapString("CONNECTED", 140, 32);
    }
}