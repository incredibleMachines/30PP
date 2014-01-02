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
        cout << "Not Connected: "<< endl;
        connected = client.connect(options);
        cout <<"Connection Status: " << connected << endl;
    }
}

//--------------------------------------------------------------
void testApp::draw(){

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
    
    Event thisEvent = * new Event(args);
    
//    cout << "Websocket onMessage: "<<endl;
//    cout << args.message <<endl;
//    
//    cout << "args: "<< endl;
//    cout<<args.json["command"].asString() <<endl;
    
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
    cout << "Sending Data on Socket"<< endl;

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
