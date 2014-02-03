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
        void setup(ofVec3f pos, ofQuaternion orientation, ofVec3f viewPos, ofVec3f viewSize, ofMesh _mesh);
    
        ofEasyCam camera;
        ofRectangle viewport;
    ofMesh mesh;
};


