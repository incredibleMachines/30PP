#include "ofMain.h"
#include "playerApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
//    ofSetupOpenGL(&window,500,500,OF_WINDOW);
    ofSetupOpenGL(&window,1920,1080,OF_FULLSCREEN);
    ofRunApp(new playerApp);

}
