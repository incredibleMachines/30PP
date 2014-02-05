//
//  Created by IncredibleMachines on 1/30/14.
//
//
// This class loads multiple camera object, which contain position, orientation and viewport data as well as their own version of a ofMesh
// Also contains key commands for selecting an active camera and altering all of the above settings using said Key Commands


#pragma once

#include "ofMain.h"

//--------CUSTOM CLASSES
#include "ModelMapper.h"

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
    
        ModelMapper map;
};
