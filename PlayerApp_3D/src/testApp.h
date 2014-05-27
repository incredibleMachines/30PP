#pragma once

#include "ofMain.h"
#include "ModelMapper/ModelMapper.h"
#include "Handlers/SocketHandler.h"

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
	void keyPressed (int key);
    void initVariables();
    
    //----------MODELS
    //ModelMapper object contains all cameras, meshes, and textures
    ModelMapper map;
    
    //total number of meshes to be loaded
    int numMesh;
    
    //---------CONTENT TEXTURE
    ofTexture * meshTexture;
    
    //---------SOCKET HANDLER
    SocketHandler socketHandler;
    bool bInited;
    
    //--------TRANSITION VARIABLES
    //which transition to trigger
    int currentTransition;
    //checking time for pausing
    bool bCheckingTime, bPausing;
    //what time in cocoa video to load
    float loadTime;
    //current end time for loop checking
    float currentEnd;
    //vector of all pause points populated on init
    vector<float> pauseTimes;
    //current pause time
    float pauseTime;
    //set loop mode
    int loopMode;
    //set number of screens required to run app
    int numScreens;
    //bools for checking screen status and restarting when proper number of screens present
    bool bScreenRestart, bStartScreenRestart;
    //variable for determining how far into a detail section to start on transition
    float startOffset;
};


