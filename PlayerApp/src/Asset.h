//
//  Asset.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/6/14.
//
//

#pragma once
#include "ofMain.h"

// Asset object
// does not need any inherent methods (as of yet)
class Asset {
    
public:
    Asset(string _eventId, string _title, string _type, string _location, string _uri);
    void update();
    void draw();
    
    string eventId;
    string type;
    string title;
    string uri;
    string location;
    
    int x;
    int y;
    int width;
    int height;
    
    ofVideoPlayer vid;
//    ofFile file;
    
private:

};