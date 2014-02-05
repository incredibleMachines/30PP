//
//  Mask.h
//  3camTest
//
//  Created by IncredibleMachines on 2/5/14.
//
//

#pragma once
#include "ofMain.h"

class Mask {
public:
    void setup();
    void draw();
    ofPolyline maskShape;
    void addVertex(ofVec2f vertex);
};