//
//  Camera.cpp
//  3camTest
//
//  Created by IncredibleMachines on 1/30/14.
//
//

/*
 Camera class. This class hold ofEasyCam object and mask polyline and path objects for each camera/monitor.
 
 methods
 - setup() pass camera data, mask polylines, and mesh data from ModelMapper JSON loader
 - addMask() create new mask object
 */


#include "Camera.h"

//--------------------------------------------------------------
void Camera::setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, vector<ofMesh> _mesh, vector<ofPolyline> _masks){
    
    //load variables into ofEasyCam camera, ofRectangle viewport and ofMesh mesh objects
    camera.setGlobalPosition(_pos);
    camera.setGlobalOrientation(_orientation);
    camera.disableMouseInput();
    
    viewport.x=_viewPos.x;
    viewport.y=_viewPos.y;
    viewport.width=_viewSize.x;
    viewport.height=_viewSize.y;
    
    mesh=_mesh;
    for (int i=0; i<mesh.size();i++){
        meshObject newObject;
        newObject.isMesh=true;
        meshObjects.push_back(newObject);
    }
    highlightMask=-1;
    for(int i=0; i<_masks.size();i++){
        ofPath tempDrawMask;
        masks.push_back(_masks[i]);
        drawMasks.push_back(tempDrawMask);
    }
}

void Camera::addMask(){
    
    //add blank mask to Camera
    ofPolyline tempMask;
    ofPath tempDrawMask;
    masks.push_back(tempMask);
    drawMasks.push_back(tempDrawMask);
    highlightMask=masks.size()-1;
}

void Camera::set2D(int _meshNum, ofPoint translate, float scale, int horizGrid, int vertGrid, ofPoint texPos, int texWidth, int texHeight, float featherRight, float featherLeft, vector< vector<ofPoint> > originals, vector< vector<ofPoint> > warped){
    
    meshObjects[_meshNum].translate=translate;
    meshObjects[_meshNum].scale=scale;
    meshObjects[_meshNum].horizGrid=horizGrid;
    meshObjects[_meshNum].vertGrid=vertGrid;
    meshObjects[_meshNum].tex.pos=texPos;
    meshObjects[_meshNum].tex.width=texWidth;
    meshObjects[_meshNum].tex.height=texHeight;
    meshObjects[_meshNum].originals=originals;
    meshObjects[_meshNum].warped=warped;
    meshObjects[_meshNum].isMesh=false;
    meshObjects[_meshNum].right=featherRight;
    meshObjects[_meshNum].left=featherLeft;
    
}