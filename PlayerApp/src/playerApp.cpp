#include "playerApp.h"

//--------------------------------------------------------------
void playerApp::setup(){
    
    options = ofxLibwebsockets::defaultClientOptions();
    options.port = 8080;
    connected = client.connect(options);
    
    client.addListener(this);
    ofSetFrameRate(60);
    
}

//--------------------------------------------------------------
void playerApp::update(){
    if(!connected){
        if(ofGetElapsedTimeMillis() % 500 <= 25){ //every half second try to reconnect
            cout << "Not Connected: "<< endl;
            connected = client.connect(options);
            cout <<"Connection Status: " << connected << endl;
        }
    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
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
void playerApp::onConnect(ofxLibwebsockets::Event &args){
    cout << "Websocket onConnect" << endl;
    
}
//--------------------------------------------------------------
void playerApp::onOpen(ofxLibwebsockets::Event &args){
    cout << "Websocket onOpen" << endl;
    
}
//--------------------------------------------------------------
void playerApp::onClose(ofxLibwebsockets::Event &args){
    cout << "Websocket onClose"<<endl;
    connected = false;
    
}
//--------------------------------------------------------------
void playerApp::onMessage(ofxLibwebsockets::Event &args){
    
    cout <<"================================"<<endl;
    cout<<"EVENT NUMBER: "<< allEvents.size() << endl;
    
    Event thisEvent = * new Event(args);
    
    allEvents.push_back(thisEvent);
    
}
//--------------------------------------------------------------
void playerApp::onBroadcast(ofxLibwebsockets::Event &args){
    cout << "Websocket onBroadcast"<< args.message << endl;
    
}
//--------------------------------------------------------------
void playerApp::onIdle(ofxLibwebsockets::Event &args){
    cout << "Websocket onIdle" << endl;
    
}
//--------------------------------------------------------------
void playerApp::keyPressed(int key){
    
    client.send("Hello World");
    cout << "Sending Data on Socket: ";

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
