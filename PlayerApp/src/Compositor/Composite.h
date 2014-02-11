//
//  Composite.h
//  PlayerApp
//
//  Created by IncredibleMachines on 2/10/14.
//
//

#pragma once

#include "ofMain.h"
#include "../Files/TextFile.h"

class Composite  {
public:
    void setup();
    void update();
    void bind();
    void unbind();
    
    ofQTKitPlayer background;
    ofImage image1;

    ofFbo drawSurface;
    TextFile textFile;
    int textX;
};

