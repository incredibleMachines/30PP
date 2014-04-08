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
#include "testApp.h"

EventHandler::EventHandler(){
    
    //allEvents = &((playerApp*)ofGetAppPtr())->events;
    eventsInited = false;
    bPlayEvent = false;
    bPlaying=false;
    
}

//--------------------------------------------------------------
void EventHandler::processEvent(string command, ofxLibwebsockets::Event &args){
    
    string thisCmd = command;
    
    //--- init assets
    if (thisCmd == INIT_CMD){
        cout << ">>> received INIT_CMD: " << thisCmd << endl;
        initEvents(args);
    }
    
    //--- play
    else if (thisCmd == PLAY_CMD){
        cout << ">>> received PLAY_CMD: " << thisCmd << endl;
        playEvent(args);
    }
    
    //--- start sales event
    else if (thisCmd == SALES_CMD){
        cout << ">>> received SALES_CMD: " << thisCmd << endl;
        //salesEvent(args);
    }
    
    //--- ambient
    else if (thisCmd == AMBIENT_CMD){
        cout << ">>> received AMBIENT_CMD: " << thisCmd << endl;
        //TODO: method
    }
    
    //--- pause
    else if (thisCmd == PAUSE_CMD){
        cout << ">>> received PAUSE_CMD: " << thisCmd << endl;
        pauseEvent(args);
    }
    
    //--- resume
    else if (thisCmd == RESUME_CMD){
        cout << ">>> received RESUME_CMD: " << thisCmd << endl;
        resumeEvent(args);
    }
    
    //--- end
    else if (thisCmd == END_CMD){
        cout << ">>> received END_CMD: " << thisCmd << endl;
        endEvent(args);
    }
    
    //--- ERROR
    else if (thisCmd == ERROR_CMD){
        cout << ">>> received ERROR_CMD: " << thisCmd << endl;
        errorEvent(args);
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
    
    movieFile = args.json["movie_file"].asString();
    cout <<"movieFilePath: " << movieFile << endl << endl;
    
    for (int e=0; e<numEvents; e++) {
        string eTitle   = args.json["events"][e]["title"].asString();
        int eStartTime  = args.json["events"][e]["start_time"].asInt();
        int eDuration   = args.json["events"][e]["duration"].asInt();
        
        cout << "event #"<< e << " title: "<< eTitle << endl;
        cout << "\tstart_time: "<< eStartTime << endl;
        cout << "\tduration: "<< eDuration << endl;
        cout << endl;
        
        //TODO: stick these in an array somewhere globally? not necessarily needed, every play event will come with all event data...
        //Event thisEvent = Event(args.json["events"].get(e, "did not find 1 complete event"));
        //allEvents.push_back(thisEvent);
    }
    cout<<"\t>>-------------- END INIT EVENT -----------------<<"<<endl;
    
    // set eventsInited true
    eventsInited = true;
}

//--------------------------------------------------------------
void EventHandler::playEvent(ofxLibwebsockets::Event &args){
    
    /* triggers the playing of a standard sales event */
    
    cout<<">>> hit playEvent()"<<endl;
    
    string eTitle   = args.json["event"]["title"].asString();
    int eStartTime  = args.json["event"]["start_time"].asInt();
    int eDuration   = args.json["event"]["duration"].asInt();
    
    //TODO: stick these in an array somewhere globally?

    if(SOCKET_DEBUG){
        cout<<"\t>>--------------START PLAY EVENT--------------<<"<<endl<<endl;
        cout << "\tevent title: "<< eTitle << endl;
        cout << "\t\tstart_time: "<< eStartTime << endl;
        cout << "\t\tduration: "<< eDuration << endl;
        cout << endl;
    }
    
    currentStart=eStartTime/1000;
    bTriggerEvent=true;
    bPlayEvent=true;
    bPlaying=true;
    

    //TODO: GOOOOOO
}

//--------------------------------------------------------------
void EventHandler::salesEvent(ofxLibwebsockets::Event &args){
    
    /* triggers the playing of SALES */
    
    cout<<">>> hit salesEvent()"<<endl;
    // cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>--------------START SALES EVENT--------------<<"<<endl<<endl;
    
//    currentStart=salesStartTime;
    bTriggerEvent=true;
    bPlayEvent=true;
    bPlaying=true;
}

//--------------------------------------------------------------
void EventHandler::pauseEvent(ofxLibwebsockets::Event &args){
    
    /* triggers the PAUSE of any playing event */
    
    cout<<">>> hit pauseEvent()"<<endl;
    // cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>-------------- PAUSE EVENT--------------<<"<<endl<<endl;
    
    bTriggerEvent=true;
    bPlayEvent=false;
    bPlaying=false;
}

//--------------------------------------------------------------
void EventHandler::resumeEvent(ofxLibwebsockets::Event &args){
    
    /* triggers the RESUME of any paused event */
    
    cout<<">>> hit resumeEvent()"<<endl;
    // cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>-------------- RESUME EVENT --------------<<"<<endl<<endl;
    
    bTriggerEvent=true;
    bPlayEvent=false;
    bPlaying=true;
}

//--------------------------------------------------------------
void EventHandler::endEvent(ofxLibwebsockets::Event &args){
    
    /* triggers the END of any play event */
    
    cout<<">>> hit endEvent()"<<endl;
    // cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>-------------- END EVENT --------------<<"<<endl<<endl;
    
//    currentStart=endTime;
    bTriggerEvent=true;
    bPlayEvent=true;
    bPlaying=true;
}

//--------------------------------------------------------------
void EventHandler::errorEvent(ofxLibwebsockets::Event &args){
    
    /* ERROR RECEIVED FROM SERVER */
    
    cout<<">>> hit error()"<<endl;
    // cout<<">>> num events received: "<< numEvents <<endl<<endl;
    cout<<"\t>>------ ERROR EVENT ---------<<"<<endl<<endl;
    
}
