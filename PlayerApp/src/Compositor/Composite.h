//
//  Composite.h
//  PlayerApp
//
//  Created by IncredibleMachines on 2/10/14.
//
//

#pragma once

#include "ofMain.h"
#include "../Files/TextFile.h"
#include "../Files/ImageFile.h"
#include "../Files/VideoFile.h"
#include "../Events/Scene.h"
#include "../Events/Asset.h"

#define SCENE_T_I_BOTH 0

#define MODEL_LEFT 0
#define MODEL_SCULPTURE 1
#define MODEL_WALL 2

class Composite  {
public:
    void setup(int _meshNum);
    void loadScene(Scene _scene);
    void update();
    void bind();
    void unbind();
    void drawFbo();
    void initScene();
    
    ofQTKitPlayer background;

    ofFbo drawSurface;
    vector<TextFile *> textFiles;
    vector<ImageFile> imageFiles;
    vector<VideoFile> videoFiles;
    Scene currentScene;
    
    int textX;
    int type;
    int meshNum;
    
    float timer;
    
    bool bFinished;
};

