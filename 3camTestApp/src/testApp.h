#pragma once

//--------------------------------------------------------------
//
//
// ADVANCED 3D EXAMPLE
//		ofNode3d, ofCamera, ofEasyCam
//
//
//--------------------------------------------------------------


#include "ofMain.h"

// Custom objects for this example
#include "Grid.h"
#include "Camera.h"

#include "ofxAssimpModelLoader.h"
#include "ofxJSONElement.h"


#define N_CAMERAS 2


#define ADJUST_MODE_CAMERA 1
#define ADJUST_MODE_VIEWPORT 4
#define ADJUST_MODE_LOOK_AT 2
#define ADJUST_MODE_MESH 3

class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void drawScene(int iCameraDraw);
        void setupCameras();
        void saveCameras();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
    
        void rotateToNormal(ofVec3f normal);
    
        
        int adjustMode;

        //CAMERA AND PLACEMENT SETTINGS
        Camera cameras[N_CAMERAS];
        ofxJSONElement settings;
        int cameraSelect;
        bool rotateSelect, scaleSelect;
        ofVec3f previous, current;
    
        //MESH SETTINGS

        ofxAssimpModelLoader test;
        int masterIndex;
        class meshVertex{
        public:
            ofVec3f vertex;
            int index;
            bool drawHighlight=false;
        };
        vector<meshVertex> moveVertices;
        bool bMouseDown;
        ofVec2f mouseDown;
        ofRectangle selectRect;
        float distThreshold, clickThreshold;
};
