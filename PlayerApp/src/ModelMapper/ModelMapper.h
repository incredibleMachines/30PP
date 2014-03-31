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
//#include "ofAVQueuePlayer.h"
//#include "../Compositor/Composite.h"

//--------INTERNAL TYPE DEFS
#define ADJUST_MODE_CAMERA 1
#define ADJUST_MODE_LOOK_AT 2
#define ADJUST_MODE_MESH 3
#define ADJUST_MODE_VIEWPORT 4
#define ADJUST_MODE_MASK 5
#define ADJUST_MODE_LOCKED 6

#define MESH_MASS 0
#define MESH_DETAIL 1

#define MAGNET_MODE_NONE 0
#define MAGNET_MODE_LINEAR 1
#define MAGNET_MODE_EASE 2

class ModelMapper {
public:
    //overloaded setup allowing different data to be passed
    void setup(int _numCams); //defaults gui cam to [0], num Meshes to 1
    void setup(int _numCams, int _guiCam); //defaults num Meshes to 1
    void setup(int _numCams, int _guiCam, int _numMeshes); // draws first _numMeshes fount
    void setup(int _numCams, int _guiCam, vector<int> _whichMeshes); //draws specifid meshes
    
    void update(vector<ofTexture *> tex);
    void draw();
    
    void keyPressed(ofKeyEventArgs& args);
    void keyReleased(ofKeyEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void mousePressed(ofMouseEventArgs& args);
    void mouseReleased(ofMouseEventArgs& args);
    void mouseMoved(ofMouseEventArgs& args);
    
    //----------GLOBAL VARIABLES
    //number of cameras, passed in setup
    int numCams;
    //which camera is guiCam, passed in setup
    int guiCam;
    //how many meshes to draw, passed in setup or calculated by .size() of vector passed in setup
    int numMeshes;
    
    //mouse variables, to deal with weirdness of using mouseX/Y with custom event listeneres
    ofVec2f mouse;
    
    //switch for drawing GUI info
    bool bDrawGui;
    //switch for drawing mesh wireframes
    bool bDrawWireframe;
    //swith for shift key modifier
    bool bShiftPressed;
    //swith for shift key modifier
    bool bCommandPressed;
    //value of move commands (modified by shift key)
    float moveModifier;
    
    
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
    //sets file path for mesh to reload
    void setMassMesh(string _reloadMesh);
    //sets file path for detailed mesh
    void setDetailMesh(string _reloadMesh);
    //move camera position
    void adjustPosition(float x, float y, float z);
    //move camera orientation
    void adjustOrientation(float x, float y, float z);
    //move mesh point
    void adjustMesh(float x, float y, float z);
    //move mask point/entire mask
    void adjustMask(float x, float y);
    //make vector of Mesh points for magnet mode
    void calculateMagnetPoints();
    
    float mapVal(float in, float inMin, float inMax, float outMin, float outMax, float shaper);
    
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
        float modifier; 
    };
    vector< vector<meshVertex> > moveVertices;
    vector< vector<meshVertex> > tempVertices;
    vector< vector<meshVertex> > magnetVertices;
    //holder for mesh filepaths
    string detailMesh;
    string massMesh;
    int meshType;
    //holder for which meshes to reload from above filepath
    vector<int> whichMeshes;
    ofPlanePrimitive plane;
    
    float magnetRadius;
    int magnetMode;
    
    
    //---------MASK SETTINGS
    class maskVertex{
    public:
        ofVec2f vertex;
        int maskIndex;
        int vertexIndex;
    };
    vector<maskVertex> maskVertices;
    bool bNewMask, bDrawingMask, bMaskPoint;
    float mouseTimer;
    
    //---------HIGHLIGHT SETTINGS
    bool bMouseDown;
    ofVec2f mouseDown;
    ofRectangle selectRect;
    float clickThreshold;
    
    //----------TEXTURE SETTINGS
    //    vector<Composite> compositeTexture;
    bool bMipMap;
    
    vector<ofTexture *> textures;
};