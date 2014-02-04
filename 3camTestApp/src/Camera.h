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
        void setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, ofMesh _mesh);
    void draw();
        ofEasyCam camera;
        ofRectangle viewport;
        ofMesh mesh;
};


