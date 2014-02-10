
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
#define MAPPER_DEBUG 0

//--------------------------------------------------------------
void playerApp::setup(){

    ofSetWindowTitle("30PP Player");
    ofSetFrameRate(60);
    
    //--- socket setup
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
    //--- modelMapper setup
    map.setup(4,0);
    map.addVideoTexture("Bonus_waves.mov");
    
//    ttFont.loadFont("fonts/nobel_reg.ttf", 18);
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
    ofSetColor(255);
//    ttFont.drawString("TESTETSTTEST", 50, 150);
}

//--------------------------------------------------------------
void playerApp::keyPressed(int key){
    
    if (key == 'i'){ // will happen in socketHandler.update() automatically when ready
        socketHandler.sendSocketCmd(INIT_REQ);
        
    }
    else if (key == 'o'){ // will happen in socketHandler.update() automatically when ready
        socketHandler.sendSocketCmd(GO_REQ);
        
    }
    else if (key == '.'){
        string test = events[0].eScenes[1].sTitle;
        cout<< "event 0: scene 1: "+test<<endl;
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
