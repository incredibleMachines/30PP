#pragma once

#include "ofMain.h"
#include "VideoThread.h"

#define NUM_VIDS 5
#define BUFFER_SIZE 3

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
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    class videoLoader{
        public:
            ofVideoPlayer video;
            bool loaded;
            bool setup;            
    };
    
    videoLoader contentBuffer[BUFFER_SIZE];
    
    string filepaths[NUM_VIDS];
    
    int play,destroy,load, count;
    
    bool bFirst,bLoaded;
    ofTexture drawTex;
    
    float timer;
    
    bool bContentLoaded, contentTriggered;
    
    int loopCount;
    
};

