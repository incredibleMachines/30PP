
#include "playerApp.h"

//--------------------------------------------------------------
void playerApp::setup(){

    ofSetWindowTitle("30PP Player");
    ofSetFrameRate(60);
    ofSetCircleResolution(60);
    
    //--- socket setup
    socketHandler.setup(8080);
    
    //--- have assets been received and loaded?
//    inited = false;
}

//--------------------------------------------------------------
void playerApp::update(){
    
    socketHandler.update();
    
    if(socketHandler.eventsInited){ //we're good to go, follow standard operating procedures
        
        
    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    socketHandler.drawDebugInfo();
    
    if(socketHandler.eventsInited){ //we're good to go, follow standard operating procedures
        
        
    }
}

//--------------------------------------------------------------
void playerApp::keyPressed(int key){
    
    if (key == 'i'){ // will happen in socketHandler.update() automatically in the future
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
