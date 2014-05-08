#pragma once

#include "ofMain.h"
#include "ModelMapper/ModelMapper.h"
#include "Handlers/SocketHandler.h"

#define BUFFER_SIZE 3
#define MESH_NUM 1
#define TEX_WIDTH 2048
#define TEX_HEIGHT 3400

#define AMBIENT_LOOP 0
#define DEFAULT_LOOP 1


class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
    void exit();
	void keyPressed  (int key);
    void initVariables();
    
	//----------COCOA DRAW SETTING
	float	lineWidth;
    
    //----------MODELS + CAMERAS
    //ModelMapper object contains all cameras, meshes, and textures
    ModelMapper map;
    
    //---------CONTENT TEXTURE
    ofTexture * meshTexture;
    
    //total number of meshes to be loaded
    int numMesh;
    
    //---------SOCKET HANDLER
    SocketHandler socketHandler;
    bool bInited;
    
    ofImage test;
    
    int currentTransition;
    bool bCheckingTime, bPausing;
    float loadTime;
    float currentEnd;
    
    vector<float> pauseTimes;
    
    float pauseTime;
    int initCount;
    
    int loopMode;
    
    int numScreens;
    bool bScreenRestart, bStartScreenRestart;
    
    float startOffset;
        
};


