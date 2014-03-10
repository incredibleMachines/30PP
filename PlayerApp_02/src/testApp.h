#pragma once

#include "ofMain.h"

#include "ModelMapper/ModelMapper.h"
#include "ofAVQueuePlayer.h"
#include "ofAVFoundationPlayer.h"

#define BUFFER_SIZE 3
#define MESH_NUM 3

class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
    void exit();
	
	void keyPressed  (int key);
	
	float	lineWidth;
	bool	doVSync;
	bool	doDisplayLink;
    void setupTexture(int _i);
    void createTexture(int _i, int _j);
    
    
    //----------MODELS + CAMERAS
    
    //ModelMapper object contains all cameras, meshes, and textures
    ModelMapper map;
    
    //----------GLOBAL FONT
    
    //single ofTrueTypeFont instance for creating ofVboMesh objects for all text loaded via SocketHandler and EventHandler
    ofTrueTypeFont masterFont;
    
    //---------CONTENT BUFFER
    
    //contentBuffer contains all loaded videos, images, and text Vbos for display on models
    
    int bBuffer;
    int bContentLoaded;
    
    class meshContent{
    public:
        ofAVQueuePlayer video;
//        ofVideoPlayer video;
        bool bSetup;
        int glType;
        int glFormat;
        int width;
        int height;
        int frames;
        bool loaded;
        unsigned char * pix;
        
    };
    
    vector<ofTexture *> meshTexture;
    
    meshContent mesh[MESH_NUM];
    
    //count of current SceneContent to load from buffer
    int count;
    
    //total number of meshes to be loaded
    int numMesh;
    
    //testing - signals that we have successfully "triggered" and init and content load event
    bool bTriggered;
    
    int play, load;
    bool bDrop;
    bool bFirstLoaded;
    bool bLoaded;
    
    
    int frameCount;
};


