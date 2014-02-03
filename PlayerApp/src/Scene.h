//
//  Scene.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/31/14.
//
//

#pragma once
#include "ofMain.h"

// Scene object
// does not need any inherent methods (as of yet)
class Scene {
    
public:
    Scene(string _eventId, string _title, string _type, string _location, string _uri);
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