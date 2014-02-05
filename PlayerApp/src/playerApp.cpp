
#include "playerApp.h"

//constants
#define INIT_CMD "{\"command\" : \"init\"}"

//--------------------------------------------------------------
void playerApp::setup(){

    ofSetWindowTitle("30PP Player");
    ofSetFrameRate(60);
    ofSetCircleResolution(60);
    
    //--- socket setup
    socketOptions = ofxLibwebsockets::defaultClientOptions();
    socketOptions.port = 8080;
    socketConnected = socketClient.connect(socketOptions);
    socketClient.addListener(this);
    
    //--- have assets been received and loaded?
    inited = false;
}

//--------------------------------------------------------------
void playerApp::update(){
    
    if(!socketConnected){ //we're not connected to controllerApp !
        if(ofGetElapsedTimeMillis() % 500 <= 20){ // ~every half second try to reconnect
            cout << "-------------------------" << endl;
            cout << "Socket Not Connected. Attempting Reconnect."<<endl;
            cout<<"Time: "<< ofGetUnixTime() << endl;
            socketConnected = socketClient.connect(socketOptions);
            cout << "Connection Status: " << socketConnected << endl;
        }
        //if (inited) { ERR: network went down AFTER app was able to init }
    }
    
    else { //socket IS connected
        if (!inited){ //we have yet to init all of our events/scenes/assets
            if(ofGetElapsedTimeMillis() % 1000 <= 20){ // ~every second request init cmd (for now)
                //cout << "-----------------------" << endl;
                //cout << "Socket Connected, Not Inited. Time: "<< ofGetUnixTime() << endl;
                //socketClient.send(INIT_CMD);
                //cout << ">>> SENT INIT_CMD ====";
            }
        }
    }
    
    if (inited){ //app is running normally, populated allAssets. network be damned, we're playing the loop.
        if(ofGetElapsedTimeMillis() % 1000 <= 20){
            //--- status update / heartbeat should go here
        }
        
        //--- update syphons
        //allSyphons.update();
    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    //--- draw some info
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString("socket status: ", 20, 30);
    ofDrawBitmapString("SPACE to request init manually", 20, ofGetHeight()-20);
    ofDrawBitmapString("socket status: ", 20, 30);
    
    if (!socketConnected){
        ofSetColor(255,0,20);
        ofDrawBitmapString("NOT CONNECTED", 140, 32);
        
    }
    else { //socketConnected
        //--- connected msg
        ofSetColor(0,255,20);
        ofDrawBitmapString("CONNECTED", 140, 32);
        
        
        if(inited){ //allAssets has been populated, follow SOP
            
            //--- draw all syphon textures
            //allSyphons.draw();
        }
    }
    
    thisImg.draw(0,0);
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

    cout <<"\n==================== SOCKET MESSAGE RECEIVED ====================="<<endl;
    string thisCmd = args.json["command"].asString();
    cout <<">>> raw message:\n" << args.message << endl;
    cout <<"======================== END SOCKET MESSAGE ========================"<<endl;
    cout <<"\n>>> thisCmd = "<< thisCmd << endl;

    //TODO: make switch cases
    
    //--- init assets
    if (thisCmd == "init"){
        cout << ">>> received init" << endl;
        initEvents(args);
    }
    
    //--- start an event
    else if (thisCmd == "start"){
        cout << ">>> received start event" << endl;
        cout<<"EVENT NUMBER: "<< allEvents.size() << endl;
        //startEvent(args); <-- TODO
    }
    
    //--- stop an event
    else if (thisCmd == "stop"){
        cout << ">>> received stop event" << endl;
        //startEvent(args); <-- TODO
    }
    
    //--- server error
    else if (thisCmd == "error"){
        cout << ">>> received server error" << endl;
        // server has returned an error
    }
    
    //--- unknown event
    else {
        cout << ">>> received UNKNOWN EVENT" << endl;
    }
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
    
   
    if (key == ' '){ //happens on setup (when inited == false)
        cout << "-----------------------" << endl;
        socketClient.send(INIT_CMD);
        cout << ">>> SENT INIT_CMD  \t||\t Time: "<< ofGetUnixTime() << endl << endl;
    }
}

//--------------------------------------------------------------
void playerApp::initEvents(ofxLibwebsockets::Event &args){ //fills the allAssets vector on init
    
    cout<<">>> hit initEvents()"<<endl;
    int numEvents = args.json["events"].size();
    cout<<">>> num events received: "<< numEvents <<endl<<endl;

    cout<<"\t>>--------------START EVENT INIT--------------<<"<<endl<<endl;
    for (int e=0; e<numEvents; e++) {
        Event thisEvent = * new Event (args.json["events"].get(e, "1 complete event"));
        cout<<endl;
        allEvents.push_back(thisEvent);
    }
    cout<<"\t>>--------------END EVENT INIT--------------<<"<<endl;
        
    //--- syphon setup
    //allSyphons.setup( allEvents ); //pass in ref to allAssets here ( might move to setup )
    
    inited = true; //we're inited, ready to go
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
