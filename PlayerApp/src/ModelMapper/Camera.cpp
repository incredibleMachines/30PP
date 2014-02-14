//
//  Camera.cpp
//  3camTest
//
//  Created by IncredibleMachines on 1/30/14.
//
//

#include "Camera.h"

//--------------------------------------------------------------
void Camera::setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, vector<ofMesh> _mesh, vector<ofPolyline> _masks){
    
    //load variables into camera, viewport and mesh objects
    
    camera.setGlobalPosition(_pos);
    camera.setGlobalOrientation(_orientation);
    camera.disableMouseInput();
    
    
    viewport.x=_viewPos.x;
    viewport.y=_viewPos.y;
    viewport.width=_viewSize.x;
    viewport.height=_viewSize.y;
    
    mesh=_mesh;
    highlightMask=-1;
    for(int i=0; i<_masks.size();i++){
        ofPath tempDrawMask;
        masks.push_back(_masks[i]);
        drawMasks.push_back(tempDrawMask);
    }
}

void Camera::addMask(){
    ofPolyline tempMask;
    ofPath tempDrawMask;
    masks.push_back(tempMask);
    drawMasks.push_back(tempDrawMask);
    highlightMask=masks.size()-1;
}