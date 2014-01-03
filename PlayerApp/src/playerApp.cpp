#include "playerApp.h"

//--------------------------------------------------------------
void playerApp::setup(){

    ofSetWindowTitle("30pp Player");
    ofSetFrameRate(60);
    ofSetCircleResolution(60);
    
    //--- socket setup
    socketOptions = ofxLibwebsockets::defaultClientOptions();
    socketOptions.port = 8080;
    socketConnected = socketClient.connect(socketOptions);
    socketClient.addListener(this);
    
    //--- syphon setup
    allContentOut.setup();
    
}

//--------------------------------------------------------------
void playerApp::update(){
    if(!socketConnected){
        if(ofGetElapsedTimeMillis() % 500 <= 25){ //every half second try to reconnect
            cout << "Not Connected: "<< endl;
            socketConnected = socketClient.connect(socketOptions);
            cout <<"Connection Status: " << socketConnected << endl;
        }
    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    //--- draw all syphon textures
    allContentOut.draw();
    
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString("socket status: ", 20, 30);
    ofDrawBitmapString("SPACE to request Event start", 20, ofGetHeight()-20);
    
    if (socketConnected){
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
    socketConnected = false;
    
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
    
    if (key == ' '){
        socketClient.send("Hello World");
        cout << "Sending Data on Socket: ";
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
