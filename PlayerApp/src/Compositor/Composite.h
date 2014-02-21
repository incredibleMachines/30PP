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
#include "ofxAVFVideoPlayer.h"

class Composite  {
public:
    void setup(int MeshNum);
    void update();
    
    //----------CUSTOM FUNCTIONS
    //load new content
    void loadScene(ofxAVFVideoPlayer * _vid);
    //bind and unbind texturereference wrappers
    void bind();
    void unbind();
    //create Fbo for wrapping around mesh in ModelMapper
    void createTexture();
    
    //----------FBO TEXTURE
    //testing - fixed video object
    ofQTKitPlayer background;
    //UV Texture for tex coordinates, calibration
    ofImage bgImage;
    //Fbo for actual draw texture
    ofTexture drawSurface;
    ofTexture drawNoMip;
    //mesh to assign this Fbo to
    int meshNum;
    //loading variable checks
    bool bFinished;
    bool bLoaded;
    bool bPlaying;
    ofVec2f pos;
    ofVec2f textPos;
    
    //pointer to content for this scene and mesh - from playerApp
    ofxAVFVideoPlayer * vid;
    ofxAVFVideoPlayer loader;
    
    //globals for storing info about our UV map at setup
    int glFormat;
    int glType;
    
    bool bSetup;
    
};

