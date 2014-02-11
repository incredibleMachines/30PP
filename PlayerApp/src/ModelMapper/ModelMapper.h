//
//  ModelMapper.h
//  3camTest
//
//  Created by IncredibleMachines on 2/5/14.
//
//

#pragma once

#include "ofMain.h"
#include "Camera.h"
#include "ofxLibwebsockets.h"
#include "ofxAssimpModelLoader.h"
#include "../Compositor/Composite.h"

//--------INTERNAL TYPE DEFS - DO NOT CHANGE
#define ADJUST_MODE_CAMERA 1
#define ADJUST_MODE_LOOK_AT 2
#define ADJUST_MODE_MESH 3
#define ADJUST_MODE_VIEWPORT 4
#define ADJUST_MODE_MASK 5
#define ADJUST_MODE_LOCKED 6

#define TEXTURE_MODE_NONE 0
#define TEXTURE_MODE_COMPOSITE 1
#define TEXTURE_MODE_VIDEO 2

class ModelMapper {
public:
    void setup(int _numCams);
    void setup(int _numCams, int _guiCam);
    void setup(int _numCams, int _guiCam, int _numMeshes);
    void update();
    void draw();
    
    void keyPressed(ofKeyEventArgs& args);
    void keyReleased(ofKeyEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void mousePressed(ofMouseEventArgs& args);
    void mouseReleased(ofMouseEventArgs& args);
    void mouseMoved(ofMouseEventArgs& args);
    
    int numCams;
    int guiCam;
    int numMeshes;
    ofVec2f mouse;
    
    //---------CUSTOM FUNCTIONS
    //drawGuiText draws information on user settings on GUI_CAMERA Screen only
    void drawGuiText();
    //draw all cameras
    void drawCameras();
    //draw Highlights for selection box, nearest point, and selected points
    void drawHighlights();
    //draw masks for selected camera
    void drawMasks();
    void updateMasks();
    //init cameras
    void setupCameras();
    //save camera data to json and meshes to .ply files
    void saveCameras();
    //add video texture to model
    void addCompositeTexture();
    
    
    //---------CAMERA SETTINGS
    int adjustMode;
    vector<Camera> cameras;
    Json::Value settings;
    int cameraSelect;
    
    //---------MESH SETTINGS
    ofxAssimpModelLoader test;
    class meshVertex{
    public:
        ofVec3f vertex;
        int index;
    };
    vector< vector<meshVertex> > moveVertices;
    vector< vector<meshVertex> > tempVertices;

    //---------MASK SETTINGS
    class maskVertex{
    public:
        ofVec2f vertex;
        int maskIndex;
        int vertexIndex;
    };
    vector<maskVertex> maskVertices;
    bool bNewMask, bDrawingMask, bMaskPoint;
    
    //---------HIGHLIGHT SETTINGS
    bool bMouseDown;
    ofVec2f mouseDown;
    ofRectangle selectRect;
    float clickThreshold;
    
    //---------VIDEO PLAYER
    vector<ofQTKitPlayer> player;
    vector<ofTexture> texture;
    vector<Composite> compositeTexture;
    
    float mouseTimer;
    bool bDrawGui;
    bool bShiftPressed;
    float moveModifier;
    bool bDrawWireframe;
    int textureMode;
    
    
    
};