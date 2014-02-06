
#include "playerApp.h"

/*
 playerApp class. Master app, contains objects for Camera, ModelMapper, Masks, SocketHandler, Events.  
 
 methods
    - setup()  init all classes and json settings
    - update() update routine on socketHandler. Standard Operating Procedures after eventsInited
    - draw()   socketDebugInfo to screen. Standard Operating Procedures after eventsInited
 
 */

//--------------------------------------------------------------
void playerApp::setup(){

    ofSetWindowTitle("30PP Player");
    ofSetFrameRate(60);
    
    //--- socket setup
    socketHandler.setup(8080, false); // (PORT,  bool verboseMode)
}

//--------------------------------------------------------------
void playerApp::update(){
    
    //--- manage sockets, connect/reconnect as needed
    socketHandler.update();
    
    if(eventsInited){   // we're good to go, follow SOP
       
    
        //everything
        
    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    socketHandler.drawDebugInfo(); //on screen socket debuggin
    
    if(eventsInited){ //we're good to go, follow standard operating procedures
        
        
        //everything
        
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
