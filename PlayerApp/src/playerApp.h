#pragma once

//openFrameworks + addons
#include "ofMain.h"

//30PP
#include "ModelMapper/ModelMapper.h"
#include "Handlers/SocketHandler.h"
#include "ofAVQueuePlayer.h"
#include "ofAVFoundationPlayer.h"
//#include "Events/Event.h"
//#include "Events/Scene.h"
//#include "Events/EventTypes.h"
//#include "Compositor/SceneContent.h"

#define BUFFER_SIZE 3
#define MESH_NUM 1

class playerApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void setupTexture(int _i);
    void createTexture(int _i, int _j);
    
    //----------WEBSOCKETS
    
    //SocketHandler object iniates parsing of all content instructions
    SocketHandler socketHandler;
    
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
    ofTexture masterTexture;
    
    meshContent mesh[MESH_NUM];
    meshContent singleMesh;

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
    bool bMipMap;
//    
//    vector <ofAVQueuePlayer *> videos;
    

    int frameCount;
    
};
