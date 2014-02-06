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
#include "ofxJSONElement.h"
#include "ofxAssimpModelLoader.h"

//--------INTERNAL TYPE DEFS - DO NOT CHANGE
#define ADJUST_MODE_CAMERA 1
#define ADJUST_MODE_LOOK_AT 2
#define ADJUST_MODE_MESH 3
#define ADJUST_MODE_VIEWPORT 4
#define ADJUST_MODE_MASK 5

class ModelMapper {
public:
    void setup(int _numCams);
    void setup(int _numCams, int _guiCam);
    void update();
    void draw();
    
    void keyPressed(ofKeyEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void mousePressed(ofMouseEventArgs& args);
    void mouseReleased(ofMouseEventArgs& args);
    void mouseMoved(ofMouseEventArgs& args);
    
    int numCams;
    int guiCam;
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
    //init cameras
    void setupCameras();
    //save camera data to json and meshes to .ply files
    void saveCameras();
    //add video texture to model
    void addVideoTexture(string videoTexture);
    
    
    //---------CAMERA SETTINGS
    int adjustMode;
    vector<Camera> cameras;
    ofxJSONElement settings;
    int cameraSelect;
    
    //---------MESH SETTINGS
    ofxAssimpModelLoader test;
    class meshVertex{
    public:
        ofVec3f vertex;
        int index;
    };
    vector<meshVertex> moveVertices;

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
    ofQTKitPlayer player;
    
    float mouseTimer;
    
};