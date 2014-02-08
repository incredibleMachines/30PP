#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    //setup GL context
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    ofBackground(0, 0, 0);
    ofDisableSeparateSpecularLight();
    
    map.setup(4,0);
    map.addVideoTexture("mapping test_01/tex/Test Map_02.mov");
    
}

//--------------------------------------------------------------
void testApp::update(){
    map.update();
    
}


//--------------------------------------------------------------
void testApp::draw(){
    map.draw();
   
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
      
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}



