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
    
    //--- start sales event
    else if (thisCmd == SALES_CMD){
        cout << ">>> received SALES_CMD: " << thisCmd << endl;
        salesEvent(args);
    }
    
    //--- sales single
    else if (thisCmd == SALES_SING_CMD){
        cout << ">>> received SALES_SING_CMD: " << thisCmd << endl;
        //TODO: method
    }
    
    //--- ambient
    else if (thisCmd == AMBIENT_CMD){
        cout << ">>> received AMBIENT_CMD: " << thisCmd << endl;
        //TODO: method
    }
    
    //--- ambient single
    else if (thisCmd == AMBIENT_SING_CMD){
        cout << ">>> received AMBIENT_SINGLE_CMD: " << thisCmd << endl;
        //TODO: method
    }
    
    //--- pause
    else if (thisCmd == PAUSE_CMD){
        cout << ">>> received PAUSE_CMD: " << thisCmd << endl;
        //TODO: method
    }
    
    //--- resume
    else if (thisCmd == RESUME_CMD){
        cout << ">>> received RESUME_CMD: " << thisCmd << endl;
        //TODO: method
    }
    
    //--- end
    else if (thisCmd == END_CMD){
        cout << ">>> received END_CMD: " << thisCmd << endl;
        //TODO: method
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
        
        //Load events into vectors. this is the info we'll be using in SceneContent
        allEvents.push_back(thisEvent);
    }
    cout<<"\t>>--------------END END EVENT INIT--------------<<"<<endl;
    
    // set eventsInited true
    eventsInited = true;
    
    for(int i=0;i<allEvents.size();i++){
        for(int j=0;j<allEvents[i].eScenes.size();j++){
            allScenes.push_back(allEvents[i].eScenes[j]);
        }
    }
}



//--------------------------------------------------------------
void EventHandler::salesEvent(ofxLibwebsockets::Event &args){
    
    /* triggers the playing of a standard sales event */
    
    cout<<">>> hit salesEvent()"<<endl;
    //cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>--------------START SALES EVEMT--------------<<"<<endl<<endl;
    
    
}


