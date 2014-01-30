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


#define N_CAMERAS 3

class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void drawScene(int iCameraDraw);
    void setupCameras();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);


    
        //test AssimpModel
        ofxAssimpModelLoader model;
		
        //my custom node
		grid nodeGrid;
    
        Camera cameras[N_CAMERAS];

    
    ofxJSONElement settings;
};
