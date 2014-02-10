#include "playerApp.h"

/*
 playerApp class. This is the master app, contains objects for Camera, ModelMapper, Masks, SocketHandler, Events.  
 
 methods
    - setup()  init all classes and json settings
    - update() update routine on socketHandler. Standard Operating Procedures after eventsInited. 
        - SOP: update modelMapper
    - draw()   socketDebugInfo to screen. Standard Operating Procedures after eventsInited
        - SOP: draw modelMapper
 */

//--- constants
#define MAPPER_DEBUG 1

//--------------------------------------------------------------
void playerApp::setup(){
    
    //setup GL context
	ofSetVerticalSync(true);
    ofSetFrameRate(60);
	ofEnableSmoothing();
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    ofEnableArbTex();
    ofBackground(0, 0, 0);
    ofDisableSeparateSpecularLight();

    ofSetWindowTitle("30PP Player");
    
    //--- socket setup
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
    //--- modelMapper setup
    map.setup(4,0,2);
    map.addVideoTexture(0,"mapping test_02/tex/Test Map_02.mov");
    map.addVideoTexture(1,"mapping test_02/tex/Test Map_02.mov");
}

//--------------------------------------------------------------
void playerApp::update(){
    
    //--- manage sockets, connect/reconnect as needed
    if(!MAPPER_DEBUG) socketHandler.update();
    
    if(eventsInited || MAPPER_DEBUG){   // we're good to go, follow standard operating procedures
        
        //everything
        
        map.update();

    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    socketHandler.drawDebugInfo(); //on screen socket debuggin
    
    if(eventsInited || MAPPER_DEBUG){ //we're good to go, follow SOP
        
        //everything
        map.draw();
        
    }
}

//--------------------------------------------------------------
void playerApp::keyPressed(int key){
    
    if (key == 'i'){ // will happen in socketHandler.update() automatically when ready
        socketHandler.sendInitCmd();
    }
}


//--------------------------------------------------------------
void playerApp::keyReleased(int key){

}

//--------------------------------------------------------------
void playerApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void playerApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void playerApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void playerApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void playerApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void playerApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void playerApp::dragEvent(ofDragInfo dragInfo){ 

}
