#include "ofMain.h"
#include "playerApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    ofSetupOpenGL(&window,500,500,OF_WINDOW);//OF_FULLSCREEN);
    ofRunApp(new playerApp);

}
