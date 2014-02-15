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
    
    //----------OF GL SETUP
    //Many of these seem to have no effect in GLFW but leaving in for now
    
    ofSetVerticalSync(true);
    ofEnableAntiAliasing;
    ofEnableDepthTest();
    ofSetFrameRate(60);
    ofEnableNormalizedTexCoords();
    ofDisableSeparateSpecularLight();
    ofBackground(0, 0, 0);
    ofSetWindowTitle("30PP Player");
    
    //----------SOCKET SETUP
    
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
    //----------MODEL MAPPER SETUP
    
    //Load font(s) for VBO rendering in SceneContent (via pointer)
    masterFont.loadFont("Nobel_book.ttf",64,true, true, true);

    //Load mesh vector to select which meshes within obj to use
    numMesh=3;
    vector<int> _meshesLoad;
    _meshesLoad.push_back(2);
    _meshesLoad.push_back(3);
    _meshesLoad.push_back(4);
    
    //setup ModelMapper - setup(number of Cameras, which camera is the gui, vector of mesh ids to draw)
    map.setup(4,0,_meshesLoad);
    
    //set path to obj file to use in setup
    map.setReloadMesh("mapping_test/Mapping test_07_02.obj");
    
    //testing - set manual trigger to false
    bTriggered=false;
    
}

//--------------------------------------------------------------
void playerApp::update(){
    
    //manage sockets, connect/reconnect as needed
    if(!MAPPER_DEBUG) socketHandler.update();
    
    if(socketHandler.eventHandler.eventsInited || MAPPER_DEBUG){   // we're good to go, follow standard operating procedures
        
        //Update Everything
            map.update();
    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    //draw socket data onscreen - TODO: Move this inside model mapper so it will display on non-[0] guiCam and will not display in ADJUST_MODE_LOCKED
    
    socketHandler.drawDebugInfo(); //on screen socket debuggin
    
    if(socketHandler.eventHandler.eventsInited || MAPPER_DEBUG){ //we're good to go, follow SOP
        
        //Draw Everything
        
        map.draw();

    }
    ofSetColor(255);

}

//--------------------------------------------------------------
void playerApp::keyPressed(int key){
    
    //manually trigger init event
    if (key == 'i'){ // will happen in socketHandler.update() automatically when ready
        socketHandler.sendSocketCmd(INIT_REQ);
    }
    
    //manually trigger go event
    else if (key == 'o'){ // will happen in socketHandler.update() automatically when ready
        socketHandler.sendSocketCmd(GO_REQ);
        
    }
    
    //printout first loaded scene title
    else if (key == '.'){
        string test = socketHandler.eventHandler.allEvents[0].eScenes[1].sTitle;
        cout<< "event 0: scene 1: "+test<<endl;
    }
    
    //manually trigger all event content loading into contentBuffer
    else if(key == '0'){
        contentBuffer.clear();
        for (int i=0;i<socketHandler.eventHandler.allEvents.size();i++){
            for(int j=0;j<socketHandler.eventHandler.allEvents[i].eScenes.size();j++){                
                SceneContent tempContent;
                tempContent.load(&socketHandler.eventHandler.allEvents[i].eScenes[j],numMesh);
                contentBuffer.push_back(tempContent);
            }
        }
        count=0;
        for(int i=0;i<numMesh;i++){
            map.compositeTexture[i].loadScene(contentBuffer[count].fullScene[i]);
        }
    }
    
    //manually switch to next scene. TODO: toss out previous contentBuffer, free memory
    else if (key== '='){
        count++;
        if(count>contentBuffer.size()-1){
            count=0;
        }
        for(int i=0;i<numMesh;i++){
            map.compositeTexture[i].loadScene(contentBuffer[count].fullScene[i]);
        }
        
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
