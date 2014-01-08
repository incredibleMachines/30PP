#include "playerApp.h"

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
    if(!socketConnected){
        if(ofGetElapsedTimeMillis() % 500 <= 20){ // ~every half second try to reconnect
            cout << "---------------------" << endl;
            cout << "Not Connected. Time: "<< ofGetElapsedTimeMillis() << endl;
            socketConnected = socketClient.connect(socketOptions);
            cout << "Connection Status: " << socketConnected << endl;
        }
        //if (inited) { ERR: network went down after app was able to initialize, filled allAssets }
    }
    
    else { //socket IS connected
        if (!inited){
            string initMsg = "{'command' : 'init'}";
            socketClient.send(initMsg);
            cout << "Sent init command:\t"+initMsg;
        }
    }
    
    if (inited){ //app is running normally, populated allAssets, network be damned, we're playing the loop.
        
        //--- update syphons
        allSyphons.update();
        
    }
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    //--- draw some info
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString("socket status: ", 20, 30);
    ofDrawBitmapString("SPACE to request Event start", 20, ofGetHeight()-20);
    
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
            allSyphons.draw();
        }
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

    cout <<"============ SOCKET MESSAGE =============="<<endl;
    string thisCmd = args.json["command"].asString();
    
    
    //TODO: make switch cases
    
    //--- init assets
    if (thisCmd == "init"){
        cout << "init all assets" << endl;
        createAssets(args);
        
    }
    
    //--- start an event
    else if (thisCmd == "start"){
        cout<<"EVENT NUMBER: "<< allEvents.size() << endl;
        Event thisEvent = * new Event(args);
        allEvents.push_back(thisEvent);
    }
    
    //--- stop an event
    else if (thisCmd == "stop"){
        // stop current event
    }
    
    //--- error
    else {
        cout<< "UKNOWN COMMAND"<<endl;
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
        string initJson = "{'command' : 'init'}";
        socketClient.send(initJson);
        cout << "Sent init command:\t"+initJson;
    }
    
    else if (key == 'a'){ //testing asset loading
    
        for(int i=0; i<5; i++){ //everything in here is the same as createAssets() --> this is just for testing
            Asset thisAsset = * new Asset ("restaurant one", "test_type", "test_loc"+ofToString(i), "/Users/jmsaavedra/30pp/culture/1.mov"); // temp Asset to push into allAssets
            // print ASSET keys
            cout<< "asset #: "<< i << "\tTitle: "<< thisAsset.title << "\tUri: "<< thisAsset.uri <<"\tType: "<< thisAsset.type << "\tLoc: "<< thisAsset.location << endl;
            allAssets.push_back(thisAsset); //push into allAssets vector
        }
        
        cout << "total assets inited: "<< allAssets.size()<<endl;
        
        //--- call syphon setup
        allSyphons.setup( allAssets ); //pass in as ref for pointer ( can move to setup()? )
        
        inited = true;
    }
}

//--------------------------------------------------------------
void playerApp::createAssets(ofxLibwebsockets::Event &args){ //fills the allAssets vector on init
    
    for (int i=0; i < args.json["assets"].size(); i++){
        // cout<< "asset #: "<< i << "   data: "<< args.json["assets"].get(i, "default") << endl; // full asset object print out
        
        string aUri      = args.json["assets"].get(i, "asset not found").get("link", "link not found").asString();
        string aTitle    = args.json["assets"].get(i, "asset not found").get("title", "title not found").asString();
        string aType     = args.json["assets"].get(i, "asset not found").get("type", "type not found").asString();
        string aLocation = args.json["assets"].get(i, "asset not found").get("location", "loc not found").asString();
        
        Asset thisAsset = * new Asset (aTitle, aType, aLocation, aUri); // pointer to temp Asset
        
        // print ASSET keys
        cout<< "asset #: "<< i << "\tTitle: "<< thisAsset.title << "\tUri: "<< thisAsset.uri <<"\tType: "<< thisAsset.type << "\tLoc: "<< thisAsset.location << endl;
        
        allAssets.push_back(thisAsset); //push into allAssets vector
    }
    
    cout << "total assets inited: "<< allAssets.size()<<endl;
        
    //--- syphon setup
    allSyphons.setup( allAssets ); //pass in ref to allAssets here ( can move to setup()? )
    
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
