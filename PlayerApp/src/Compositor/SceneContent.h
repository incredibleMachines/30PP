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
    
    //----------CONTENT ORGANIZATION
    
    //Content structs for Image, Text, and Video
    class ImageContent{
        public:
            ofImage image;
            ofVec2f coords;
            bool fullScreen=false;
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
            bool fullScreen=false;
    };
   
    //content struct of vectors of the above structs for a single mesh. TODO: decide whether this should be by mesh or one huge texture for all meshes
    class meshScene{
    public:
        vector<VideoContent> vid;
        vector<ImageContent> img;
        vector<TextContent> txt;
    };
    
    //variable identifying which mesh to load into
    int currentMesh;
    
    //content struct of vector of the above for all meshes
    vector<meshScene> fullScene;
    
    //----------POINTERS
    
    //pointer to font for creating ofVboMeshes
    ofTrueTypeFont * ttFont;
    
};