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

#define NUM_TEXTURES 12

class Syphons {
    
public:
    Syphons();
    void setup();
    void draw();
    void publishAll();
    

private:
    ofxSyphonServer server[NUM_TEXTURES];
    ofTexture tex[NUM_TEXTURES];
    
};