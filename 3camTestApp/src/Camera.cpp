//
//  Camera.cpp
//  3camTest
//
//  Created by IncredibleMachines on 1/30/14.
//
//
// This class holds camera and viewport data loaded out of JSON file in testApp setup.
// Method setup creates camera object with global position and create viewport ofRect for that camera.

#include "Camera.h"

//--------------------------------------------------------------
void Camera::setup(ofVec3f pos, ofQuaternion orientation, ofVec3f viewPos, ofVec3f viewSize, ofMesh _mesh){

    camera.setGlobalPosition(pos);
    camera.setGlobalOrientation(orientation);
    camera.disableMouseInput();
    
    viewport.x=viewPos.x;
    viewport.y=viewPos.y;
    viewport.width=viewSize.x;
    viewport.height=viewSize.y;
    
    mesh=_mesh;
    
}
