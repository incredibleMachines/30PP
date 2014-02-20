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

class SceneContent  {
public:
    
    void load(Scene * scene, int _numMeshes);
    
    void loadText(int i);
    void loadImage(int i);
    void loadVideo(int i);
    
    //----------CONTENT ORGANIZATION

   
    //content struct of vectors of the above structs for a single mesh. TODO: decide whether this should be by mesh or one huge texture for all meshes
    class meshScene{
    public:
        vector<ofVideoPlayer> vid;
    };
    
    //variable identifying which mesh to load into
    int currentMesh;
    
    //content struct of vector of the above for all meshes
    vector<meshScene> fullScene;
    
    //----------POINTERS
    
    Scene * scene;
    int numMesh;
    
};