//
//  Created by IncredibleMachines on 1/30/14.
//
//
// This class loads multiple camera object, which contain position, orientation and viewport data as well as their own version of a ofMesh
// Also contains key commands for selecting an active camera and altering all of the above settings using said Key Commands


#pragma once

#include "ofMain.h"

//Necessary addons, currently stored in of addons folder, not locally
#include "ofxAssimpModelLoader.h"
#include "ofxJSONElement.h"

//--------CUSTOM CLASSES
#include "Camera.h"

//--------USER DEFINABLE VARIABLES
//N_CAMERAS is total number of cameras/viewports
#define N_CAMERAS 2

//GUI_CAMERA is
#define GUI_CAMERA 0

//--------INTERNAL TYPE DEFS - DO NOT CHANGE
#define ADJUST_MODE_CAMERA 1
#define ADJUST_MODE_VIEWPORT 4
#define ADJUST_MODE_LOOK_AT 2
#define ADJUST_MODE_MESH 3

class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
    
        //---------CUSTOM FUNCTIONS
        //drawGuiText draws information on user settings on GUI_CAMERA Screen only
        void drawGuiText();
        //draw all cameras
        void drawCameras();
        //draw Highlights for selection box, nearest point, and selected points
        void drawHighlights();
        //init cameras
        void setupCameras();
        //save camera data to json and meshes to .ply files
        void saveCameras();
    

        //---------CAMERA SETTINGS
        int adjustMode;
        Camera cameras[N_CAMERAS];
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
    
        //---------HIGHLIGHT SETTINGS
        bool bMouseDown;
        ofVec2f mouseDown;
        ofRectangle selectRect;
        float clickThreshold;
    
        //---------VIDEO PLAYER
        ofQTKitPlayer player;
};
