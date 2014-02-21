//
//  SceneContent.h
//  PlayerApp
//
//  Created by IncredibleMachines on 2/12/14.
//
//

#pragma once
#include "ofMain.h"
#include "../Events/Scene.h"
#include "ZoneTypes.h"
#include "ofxAVFVideoPlayer.h"

class SceneContent  {
public:
    
    void load(Scene * scene, int _numMeshes);
    void update();
    
    void loadText(int i);
    void loadImage(int i);
    void loadVideo(int i);
    
    //----------CONTENT ORGANIZATION
    
    //variable identifying which mesh to load into
    int currentMesh;
    
    //content struct of vector of the above for all meshes
    vector<ofxAVFVideoPlayer *> fullScene;
    
    //----------POINTERS
    
    Scene * scene;
    int numMesh;
    
};