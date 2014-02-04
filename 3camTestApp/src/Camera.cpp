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
void Camera::setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, ofMesh _mesh){

    camera.setGlobalPosition(_pos);
    camera.setGlobalOrientation(_orientation);
    camera.disableMouseInput();
    
    viewport.x=_viewPos.x;
    viewport.y=_viewPos.y;
    viewport.width=_viewSize.x;
    viewport.height=_viewSize.y;
    
    mesh=_mesh;
    
}
