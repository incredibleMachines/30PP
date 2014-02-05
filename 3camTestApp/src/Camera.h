//
//  Camera.h
//  3camTest
//
//  Created by IncredibleMachines on 1/30/14.
//
//
// This class holds camera and viewport data loaded out of JSON file in testApp setup.
// Method setup creates camera object with global position and create viewport ofRect for that camera.

#pragma once
#include "ofMain.h"
#include "Mask.h"

class Camera {
    public:

        void setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, ofMesh _mesh);
        void addMask();
    
        ofEasyCam camera;
        ofRectangle viewport;
        ofMesh mesh;
        vector<Mask> mask;
};


