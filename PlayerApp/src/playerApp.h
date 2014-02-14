#pragma once

//openFrameworks + addons
#include "ofMain.h"

//30PP
#include "ModelMapper/ModelMapper.h"
#include "Handlers/SocketHandler.h"
#include "Events/Event.h"
#include "Events/Scene.h"
#include "Events/EventTypes.h"
#include "Compositor/SceneContent.h"


class playerApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    SocketHandler socketHandler;
    
//    bool eventsInited;
    
    ModelMapper map;
    
    ofTrueTypeFont masterFont;//testing
    
    vector <string> allImages;
    
    bool bTriggered;
    
    vector<SceneContent> contentBuffer;
    int count;
    int numMesh;
};
