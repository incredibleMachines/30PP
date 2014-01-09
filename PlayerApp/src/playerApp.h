#pragma once

//openFrameworks + addons
#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include "ofxSyphon.h"

//30PP
#include "Event.h"
#include "Syphons.h"

class playerApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void initEvents(ofxLibwebsockets::Event &args);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //Web Sockets
    ofxLibwebsockets::Client socketClient;
    ofxLibwebsockets::ClientOptions socketOptions;
    bool socketConnected;

    void onConnect(ofxLibwebsockets::Event& args);
    void onOpen(ofxLibwebsockets::Event& args);
    void onClose(ofxLibwebsockets::Event& args);
    void onIdle(ofxLibwebsockets::Event& args);
    void onMessage(ofxLibwebsockets::Event& args);
    void onBroadcast(ofxLibwebsockets::Event& args);
    
    bool inited;

    vector <Event> allEvents;
    
    Syphons allSyphons;
    
};
