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
#include "ofxTween.h"
#include "ofxUI.h"
#include "ofxHomography.h"
//#include "ofAVQueuePlayer.h"
//#include "../Compositor/Composite.h"

//--------INTERNAL TYPE DEFS
#define ADJUST_MODE_CAMERA 1
#define ADJUST_MODE_LOOK_AT 2
#define ADJUST_MODE_VIEWPORT 3
#define ADJUST_MODE_MESH 4
#define ADJUST_MODE_2D 5
#define ADJUST_MODE_MASK 6
#define ADJUST_MODE_LOCKED 7

#define MESH_2D 0
#define MESH_3D 1

#define SELECT_MODE_POINTER 0
#define SELECT_MODE_PAINTBRUSH 1
#define SELECT_MODE_PEN 2
#define SELECT_MODE_DOUBLE_PEN 3

#define EASE_MODE_NONE 0
#define EASE_MODE_IN 1
#define EASE_MODE_OUT 2
#define EASE_MODE_BOTH 3
#define EASE_MODE_SEPARATE 4

#define EASE_STYLE_LINEAR 0
#define EASE_STYLE_QUAD 1
#define EASE_STYLE_CUBIC 2
#define EASE_STYLE_QUART 3
#define EASE_STYLE_EXPO 4
#define EASE_STYLE_SINE 5
#define EASE_STYLE_CIRC 6

#define TRANSITION_DEFAULT 0
#define TRANSITION_GASTRONOMY 1
#define TRANSITION_MARKETS 2
#define TRANSITION_SHOPPING 3
#define TRANSITION_ARTS 4
#define TRANSITION_LEISURE 5
#define TRANSITION_END 6
#define TRANSITION_GRADIENT 7
#define TRANSITION_CLOUDS 8
#define TRANSITION_PARTY 9
#define TRANSITION_WAVES 10
#define TRANSITION_CHILDREN 11

class ModelMapper {
public:
    //pass in number of cameras, which one is gui, and vector of meshes with info about 2D or 3D
    void setup(int _numCams, int _guiCam, vector< vector<int> > _whichMeshes, int width, int heigh); //draws specifid meshes
    
    //pass in current texture reference
    void update(ofTexture * tex);
    void draw();
    
    //default ofEvents for ModelMapper
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
    //which mesh to edit
    int currentMesh;
    
    //mouse variables, to deal with weirdness of using mouseX/Y with custom event listeneres
    ofVec2f mouse;
    
    //switch for drawing GUI info
    bool bDrawGui;
    //switch for drawing mesh wireframes
    bool bDrawWireframe;
    //switch for drawing mesh point highlights
    bool bDrawHighlights;
    //switch for shift key modifier
    bool bShiftPressed;
    //switch for cmd key modifier
    bool bCommandPressed;
    //value of move commands (modified by shift/cmd key)
    float moveModifier;
    //switch for separate GUICam
    bool bGuiCamAdjust;
    //switch for screen lock status
    bool bLocked;
    bool bUnlocked;
    
    
    //---------DRAW FUNCTIONS
    //draw all cameras
    void drawCameras();
    //draw Highlights for selection box, nearest point, and selected points
    void drawHighlights();
    //draw masks for selected camera, called from within drawCameras
    void drawMasks(int i);
    //set LockedMode
    void setLocked(bool locked);
    
    //---------SETUP FUNCTIONS
    //init cameras from json
    void setupCameras();
    //save camera data to json and meshes to .ply files for all cameras
    void saveCameras();
    //save camera data to json and meshes to .ply files for one cameras
    void saveCamera(int n);
    //sets file path for mesh to reload
    void setMassMesh(string _reloadMesh);
    //set mesh to draw 2D homography instead of 3D mesh
    void set2D(int _meshNum);
    //turn all gui tabs on and off
    void setGUIVisible(bool hide);
    //setup all GUI elements
    void setupGUI();
    
    //---------ADJUST FUNCTIONS
        //make vector of Mesh points for magnet mode
    void calculateMagnetPoints();
    //do magnet mapping using ofxTween easing functions
    float magnetMap(float distance, float radius);
    //update mask points from ofPolyline into ofPath when changes is made
    void updateMasks();
    //reset selected 3D points back to 2D mask
    void resetSelected();
    //clear selection of points
    void clearSelection();
    
    void adjustMesh(float x, float y, float z);
    
    //---------CAMERA SETTINGS
    int adjustMode;
    vector<Camera> cameras;
    Json::Value settings;
    int cameraSelect;
    
    //---------MESH SETTINGS
    //3D vertex custom class
    class meshVertex{
    public:
        ofVec3f vertex;
        int index;
        float modifier;
        float distance;
    };
    
    //2D vertex custom class
    class vertex2D{
    public:
        ofVec3f vertex;
        int box;
        int index;
    };
    
    //selected 3D vertices via user clicks
    vector< vector<meshVertex> > moveVertices;
    vector< vector<meshVertex> > tempVertices;
    vector<meshVertex> surroundingVertices;
    
    //selected vertices via magnet and pen tools
    vector< vector<meshVertex> > magnetVertices;
    
    //DOUBLE PEN TOOL
    vector< vector<meshVertex> > tempDoubleVertices;
    vector< vector<meshVertex> > doubleVertices;
    
    //selected vertices in 2D
    vector< vector<vertex2D> > vertices2D;
    vector< vector<vertex2D> > tempVertices2D;
    
    //holder for reload mesh filepaths
    string massMesh;
    
    //holder for which meshes to reload from above filepath
    vector<vector<int> > whichMeshes;
    
    //switches for pen and double pen status
    bool bDrawingPen, bNewPen, bDrawingDouble, bNewDouble;
    
    //polyline for storing pen and double pen shapes
    ofPolyline penPoly, doublePoly;
    
    //radius of magnet selection in pointer and paintbrush modes
    float magnetRadius;
    
    //setting of magnet select mode, easing mode, and style of easing on in/out/both
    int selectMode, easeMode, easeInStyle, easeOutStyle, easeBothStyle;
    
    //radius of paintbrush selection tool
    int brushRadius;
    
    //ofxTween easing types for magnet mode
    ofxEasingLinear easeLinear;
    ofxEasingQuad easeQuad;
    ofxEasingCubic easeCubic;
    ofxEasingQuart easeQuart;
    ofxEasingExpo easeExpo;
    ofxEasingSine easeSine;
    ofxEasingCirc easeCirc;
    
    
    //---------MASK SETTINGS
    //custom class for mask verties
    class maskVertex{
    public:
        ofVec2f vertex;
        int maskIndex;
        int vertexIndex;
    };
    
    //mask vertices
    vector<maskVertex> maskVertices;
    
    //switches for mask shape drawing status
    bool bNewMask, bDrawingMask, bMaskPoint;
    
    //mesh pen vertices
    vector<maskVertex> penVertices;
    
    //---------HIGHLIGHT SETTINGS
    //check on if mouse is pressed
    bool bMouseDown;
    
    //mouse press position
    ofVec2f mouseDown;
    
    //drag rectangle draw
    ofRectangle selectRect;
    
    //timer for checking double clicks
    float mouseTimer;
    
    //distance from vertices for clicks to register
    float clickThreshold;
    
    //----------TEXTURE SETTINGS
    //UV texture reference for global texture that is mapped to all meshes
    ofTexture * texture;
    
    
    //-----------GUI SETTINGS
    //gui event handler
    void guiEvent(ofxUIEventArgs &e);
    
    //global vectors to populate radial boxes in guis
    vector<string> adjustModes;
    vector<string> cameraNames;
    vector<string> easeMethods;
    
    //main gui page gui, setup fuction, and globals
    ofxUISuperCanvas *mainGUI;
    void setMainGUI();
    ofxUIButton *drawMesh0;
    ofxUIButton *drawMesh1;
    ofxUIButton *drawMesh2;
    ofxUIButton *drawMesh3;
    ofxUIRadio *currentMode;
    ofxUIButton *wireframeButton;
    ofxUIButton *performanceButton;
    ofxUITextInput *fps;
    
    //camera position gui, setup fuction, and globals
    void setPositionGUI();
    ofxUISuperCanvas *positionGUI;
    ofxUITextInput *positionX;
    ofxUITextInput *positionY;
    ofxUITextInput *positionZ;
    ofxUITextInput *rotateZ;

    //camera orientation gui, setup fuction, and globals
    ofxUISuperCanvas *orientationGUI;
    void setOrientationGUI();
    ofxUITextInput *orientationX;
    ofxUITextInput *orientationY;
    ofxUITextInput *orientationZ;
    ofxUITextInput *orientationW;
    
    //camera viewport gui, setup fuction, and globals
    ofxUISuperCanvas *viewportGUI;
    void setViewportGUI();
    ofxUITextInput *viewportX;
    ofxUITextInput *viewportY;
    
    //mesh general gui, setup fuction, and globals
    ofxUISuperCanvas *meshGUI;
    void setMeshGUI();
    
    //easing gui, setup function, hide function, dynamic drawing function, and globals
    ofxUISuperCanvas *magnetGUI;
    void setMagnetGUI();
    void hideMagnetTypes();
    void setEaseHeights(bool radiusSpacer, bool pen1, bool pen2, bool brushRadius);
    ofxUILabel *magnetRadiusLabel;
    ofxUITextInput *magnetRadiusSet;
    ofxUILabel *brushRadiusLabel;
    ofxUITextInput *brushRadiusSet;
    ofxUISpacer *magnetRadiusSpacer;
    ofxUIButton *penButton;
    ofxUIButton *doublePenButton;
    ofxUILabel *easeTypeLabel;
    ofxUIRadio *easeTypeRadio;
    ofxUISpacer *easeTypeSpacer;
    ofxUILabel *easeInLabel;
    ofxUIRadio *easeInMethod;
    ofxUILabel *easeOutLabel;
    ofxUIRadio *easeOutMethod;
    ofxUILabel *easeBothLabel;
    ofxUIRadio *easeBothMethod;
    //variables for dynamically drawing in easing radio buttons
    int easeHeight, easeTypeHeight;

    
    //mesh 2D "madmapper style" gui, setup fuction, and globals
    ofxUISuperCanvas *mesh2DGUI;
    void setMesh2DGUI();
    ofxUITextInput *translateX2D;
    ofxUITextInput *translateY2D;
    ofxUITextInput *scale2D;
    ofxUITextInput *featherRight2D;
    ofxUITextInput *featherLeft2D;
    
    //mash gui, setup function, and global
    ofxUISuperCanvas *maskGUI;
    void setMaskGUI();
    ofxUIButton *maskButton;
    
    //----------------TRANSITIONS
    //trigger transition, called in testApp in response to eventHandler
    void fadeIn(int type);
    //set for duration of transition
    bool bTransitioning;
    //set for fading in
    bool bTransitionStarted;
    //set for hold time to load video behind
    bool bTransitionLoading;
    //set for fading out
    bool bTransitionFinished;

    //length of transition
    int transitionTime;
    //length of hold time for background video loading
    int loadTime;
    //timer on transition
    int transitionTimer;
    //textures for each individual transition graphic
    ofTexture defaultFrame, gastronomyFrame, marketsFrame, shoppingFrame, artsFrame, leisureFrame, ambientGradientFrame, endFrame, ambientCloudsFrame, ambientPartyFrame, ambientWavesFrame, childrenFrame;
    //reference for current transition graphic
    ofTexture * fadeFrame;
    
    bool bEnableGuiCam;
    
    bool bClearing;
    float clearTime;
    
    bool warped[4][4];
    
    void adjustRoll(float zR);
    
    void calculateGuiValues();
    
    void findMeshFaces(int i, int j); 
    
    void adjust2D(float x, float y);
    void adjustMask(float x, float y);
    
    void drawFades(int i);
    
};