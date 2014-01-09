//
//  Event.h
//  PlayerApp
//  Created by Joseph Saavedra on 1/2/14.
/*
 
 Event Object

 gets instantiated on each incoming Event object, all fields + associated Asset object fields are populated,
 (TODO: load assets)
 
*/
//

#pragma once
#include "ofMain.h"
#include "Asset.h"
#include "ofxLibwebsockets.h"


// Asset object
// >>> moved into it's own class
//class Asset {
//public:
//    string aType;
//    string aTitle;
//    string aLink;
//    string aLocation;
//};



// Event class
class Event {
public:
    
    Event(int eventIndex, ofxLibwebsockets::Event &args);
    //void initEvent(ofxLibwebsockets::Event &args);
    
    string eCommand;
    string eTitle;
    string eDuration;
    string eAnimType;
    string eId;
    string eCreatedAt;
    
    vector <Asset> eAssets; //to contain a copy of allAssets vector from playerApp
    
private:
    
};

