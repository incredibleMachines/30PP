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
class Scene {
    
    public:
        Scene(Json::Value thisScene);
    
        int sZoneType;
        string sTitle;
        string sText;
        string sCreatedAt;
    
        vector <Asset> sAssets; //sceneAssets
    
private:
    
};