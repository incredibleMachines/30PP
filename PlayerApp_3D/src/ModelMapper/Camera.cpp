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
void Camera::setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, vector<ofMesh> _mesh, vector<ofPolyline> _masks, vector<bool> _which, ofPoint warped[4][4], float _rotate, bool cam3){
    
    //load variables into ofEasyCam camera, ofRectangle viewport and ofMesh mesh objects

//    meshTranslate=_pos;
    
    viewport.x=_viewPos.x;
    viewport.y=_viewPos.y;
    viewport.width=_viewSize.x;
    viewport.height=_viewSize.y;
    
    which=_which;
    mesh=_mesh;

    
    for (int i=0; i<mesh.size();i++){
        meshObject newObject;
        newObject.isMesh=true;
        meshObjects.push_back(newObject);
        vector<ofPoint> tempPoints;
    }
    
    highlightMask=-1;
    for(int i=0; i<_masks.size();i++){
        ofPath tempDrawMask;
        masks.push_back(_masks[i]);
        drawMasks.push_back(tempDrawMask);
    }
    
    originals3D[3][0]=ofPoint(0, 0);
    originals3D[3][1]=ofPoint(1920, 0);
    originals3D[3][2]=ofPoint(1920, 1080);
    originals3D[3][3]=ofPoint(0, 1080);
    
    warped3D[3][0]=warped[3][0];
    warped3D[3][1]=warped[3][1];
    warped3D[3][2]=warped[3][2];
    warped3D[3][3]=warped[3][3];
    
    cameraView.allocate(1920,1080,GL_RGBA);
    
    pos = _pos;
    camera.roll(_rotate);
    rotate=_rotate;
//    camera.setGlobalPosition(_pos);
    if(cam3==true){
        setTarget();
    }
    else{
        setTarget1();
    }

    //    camera.setGlobalOrientation(_orientation);
    camera.disableMouseInput();
    camera.disableOrtho();
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

void Camera::reset(){
    camera.setGlobalPosition(ofVec3f(mesh[3].getCentroid().x+10,15,mesh[3].getCentroid().z+397));
    camera.setTarget(ofVec3f(mesh[3].getCentroid().x+30,mesh[3].getCentroid().y, mesh[3].getCentroid().z));
    rotate=0;
}

void Camera::setTarget(){
    camera.setGlobalPosition(ofVec3f(mesh[3].getCentroid().x+5,-17,pos.z));
    camera.setTarget(ofVec3f(mesh[3].getCentroid().x+30,mesh[3].getCentroid().y, mesh[3].getCentroid().z));
    camera.roll(rotate);
}

void Camera::setTarget1(){
    camera.setGlobalPosition(ofVec3f(-350,-135,mesh[3].getCentroid().z+397));
    camera.setTarget(ofVec3f(-385,-135 , mesh[3].getCentroid().z));
    camera.roll(rotate);
}