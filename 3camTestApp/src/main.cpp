#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    
    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    ofSetupOpenGL(&window,2880,1800,OF_FULLSCREEN);
    ofRunApp(new testApp);
    
}

