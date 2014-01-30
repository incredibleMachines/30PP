//
//  Camera.h
//  3camTest
//
//  Created by IncredibleMachines on 1/30/14.
//
//

#pragma once
#include "ofMain.h"

class Camera {
    public:
        void setup(ofVec3f camPos, ofVec3f lookPos, ofVec3f viewPos, ofVec3f viewSize);

        ofEasyCam camera;
        ofRectangle viewport;
    
};


