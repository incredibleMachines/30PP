//
//  EventHandler.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/7/14.
//
//
/*
 
 EventHandler class
 
 listens for commands from the SocketHandler and executes Event stuff
 
    - initEvents: kicks off Init of all Event, Scene, and Asset. Vectors are filled for all.
    - salesEvent: triggers playing of a standard sales event
    - fadeIn (intro)
    - fadeOut (outro)
 
 */

#include "EventHandler.h"
#include "playerApp.h"

EventHandler::EventHandler(){
    
    //allEvents = &((playerApp*)ofGetAppPtr())->events;
    eventsInited = false;
    
}

//--------------------------------------------------------------
void EventHandler::processEvent(string command, ofxLibwebsockets::Event &args){
    
    string thisCmd = command;
    
    //--- init assets
    if (thisCmd == INIT_CMD){
        cout << ">>> received INIT_CMD: " << thisCmd << endl;
        initEvents(args);
    }
    
    //--- start an event
    else if (thisCmd == SALES_CMD){
        cout << ">>> received SALES_CMD: " << thisCmd << endl;
        salesEvent(args);
    }
    
    //--- outro
    else if (thisCmd == OUTRO_CMD){
        cout << ">>> received OUTRO_CMD: " << thisCmd << endl;
        //outroEvent(args); <-- TODO
    }
    
    //--- ERROR
    else if (thisCmd == ERROR_CMD){
        cout << ">>> received ERROR_CMD: " << thisCmd << endl;
        // server has returned an error
    }
    
    //--- unknown event
    else {
        cout << ">>> received UNKNOWN EVENT: " << thisCmd << endl;
    }
}

//--------------------------------------------------------------
void EventHandler::initEvents(ofxLibwebsockets::Event &args){
    
    /* kicks off Init of all Event, Scene, and Asset. Vectors are filled for all. */
    
    int numEvents = args.json["events"].size();
    
    cout<<">>> hit initEvents()"<<endl;
    cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>--------------START ALL EVENT INIT--------------<<"<<endl<<endl;
    
    for (int e=0; e<numEvents; e++) {
        Event thisEvent = Event(args.json["events"].get(e, "did not find 1 complete event"));
        cout<<endl;
//        ((playerApp*)ofGetAppPtr())->events.push_back(thisEvent);
        allEvents.push_back(thisEvent);
    }
    cout<<"\t>>--------------END END EVENT INIT--------------<<"<<endl;
    
    // set eventsInited true
//    ((playerApp*)ofGetAppPtr())->eventsInited = true;
    eventsInited = true;
}



//--------------------------------------------------------------
void EventHandler::salesEvent(ofxLibwebsockets::Event &args){
    
    /* triggers the playing of a standard sales event */
    
    cout<<">>> hit salesEvent()"<<endl;
    //cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>--------------START SALES EVEMT--------------<<"<<endl<<endl;
    
    
}


