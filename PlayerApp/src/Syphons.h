//
//  Syphons.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "Asset.h"
#include "Event.h"

// hardcoded based on number of projection outputs
#define NUM_SYPHONS 8

class Syphons {
    
public:
    Syphons();
    void setup( vector <Asset> & _pAssets );
    void update();
    void draw();
    void publishAll();
    

private:
    
    ofxSyphonServer server [NUM_SYPHONS];
    
    ofFbo fbo [NUM_SYPHONS];
    
    //ofTexture tex [NUM_SYPHONS];
    //ofVboMesh vboMesh;
    
    vector <Asset> * pAsset; //<-- referenced from playerApp
    
    int numAssets; // number of total assets
};




