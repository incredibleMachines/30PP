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

class Camera {
    public:

        void setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, vector<ofMesh> _mesh, vector<ofPolyline> _masks);
        void addMask();
    
        ofEasyCam camera;
        ofRectangle viewport;
        vector<ofMesh> mesh;
        
        vector<ofPolyline> masks;
        int highlightMask;
};


