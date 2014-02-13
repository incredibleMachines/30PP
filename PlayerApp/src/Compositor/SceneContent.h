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

class SceneContent  {
public:
    
    void load(Scene * _scene, int _numMeshes);
    
    class ImageContent{
        public:
            ofImage image;
            ofVec2f coords;
    };
    
    class TextContent{
        public:
            ofVboMesh text;
            ofVec2f coords;
    };
    
    class VideoContent{
        public:
            ofVideoPlayer video;
            ofVec2f coords;
    };
    
    class meshScene{
    public:
        vector<VideoContent> vid;
        vector<ImageContent> img;
        vector<TextContent> txt;
    };
    
    vector<meshScene> fullScene;
    
    ofTrueTypeFont * ttFont;
    Scene * scene;
    
};