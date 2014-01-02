//
//  Event.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxLibwebsockets.h"


// Asset object
// does not need any inherent methods (as of yet)
class Asset {
public:
    string aType;
    string aTitle;
    string aLink;
    string aLocation;
};



// Event class
class Event {
public:
    
    Event(ofxLibwebsockets::Event &args);
    //void initEvent(ofxLibwebsockets::Event &args);
    
    
    string eCommand;
    string eTitle;
    string eDuration;
    vector <Asset> eAssets;
    
private:
    
};

