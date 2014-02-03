//
//  Scene.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/31/14.
//
//

#pragma once
#include "ofMain.h"
#include "Asset.h"
#include "ofxLibwebsockets.h"

// Scene object
// does not need any inherent methods (as of yet)
class Scene {
    
public:
    //Scene(string _eventId, string _title, string _type, string _location, string _uri);
    Scene(Json::Value thisScene);
    void update();
    void draw();
    
    string sType;
    string sTitle;
    
    vector <Asset> sAssets;
    
private:
    
};