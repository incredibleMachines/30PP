//
//  Composite.h
//  PlayerApp
//
//  Created by IncredibleMachines on 2/10/14.
//
//

#pragma once

#include "ofMain.h"
#include "../Events/Scene.h"
#include "../Events/Asset.h"
#include "SceneContent.h"

#define SCENE_T_I_BOTH 0

#define MODEL_LEFT 0
#define MODEL_SCULPTURE 1
#define MODEL_WALL 2

class Composite  {
public:
    void setup();
    void loadScene(SceneContent::meshScene &_scene);
    void update();
    void bind();
    void unbind();
    void drawFbo();
    void initScene();
    
    ofQTKitPlayer background;
    ofImage bgImage;

    ofFbo drawSurface;
    
    int textX;
    int type;
    int meshNum;
    
    float timer;
    
    bool bFinished;
    bool bLoaded;
    bool bPlaying;
    
    SceneContent::meshScene * currentScene;
};

