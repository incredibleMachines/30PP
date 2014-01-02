#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    options = ofxLibwebsockets::defaultClientOptions();
    options.port = 8080;
    connected = client.connect(options);
    
    client.addListener(this);
    ofSetFrameRate(60);
    
}

//--------------------------------------------------------------
void testApp::update(){
    if(!connected){
        if(ofGetElapsedTimeMillis() % 500 <= 25){ //every half second try to reconnect
            cout << "Not Connected: "<< endl;
            connected = client.connect(options);
            cout <<"Connection Status: " << connected << endl;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString("socket status: ", 20, 30);
    
    if (connected){
        ofSetColor(0,255,20);
        ofDrawBitmapString("CONNECTED", 140, 32);
    } else {
        ofSetColor(255,0,20);
        ofDrawBitmapString("NOT CONNECTED", 140, 32);
    }
}
//--------------------------------------------------------------
void testApp::onConnect(ofxLibwebsockets::Event &args){
    cout << "Websocket onConnect" << endl;
    
}
//--------------------------------------------------------------
void testApp::onOpen(ofxLibwebsockets::Event &args){
    cout << "Websocket onOpen" << endl;
    
}
//--------------------------------------------------------------
void testApp::onClose(ofxLibwebsockets::Event &args){
    cout << "Websocket onClose"<<endl;
    connected = false;
    
}
//--------------------------------------------------------------
void testApp::onMessage(ofxLibwebsockets::Event &args){
    
    cout <<"================================"<<endl;
    cout<<"EVENT NUMBER: "<< allEvents.size() << endl;
    
    Event thisEvent = * new Event(args);
    
    allEvents.push_back(thisEvent);
    
}
//--------------------------------------------------------------
void testApp::onBroadcast(ofxLibwebsockets::Event &args){
    cout << "Websocket onBroadcast"<< args.message << endl;
    
}
//--------------------------------------------------------------
void testApp::onIdle(ofxLibwebsockets::Event &args){
    cout << "Websocket onIdle" << endl;
    
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    client.send("Hello World");
    cout << "Sending Data on Socket: ";

}
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
