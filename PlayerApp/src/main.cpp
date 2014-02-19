#include "ofMain.h"
#include "playerApp.h"
#include "ofAppGLFWWindow.h"
//#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGLFWWindow window;

    //GLFW GL SETUP
    //Don't seem to affect the FBO textures as these have their own GL settings but leaving them in for now as reference
    
    window.setMultiDisplayFullscreen(true);
    window.setDoubleBuffering(true);
    window.setVerticalSync(true);
    window.setDepthBits(8);
    window.setColorBits(8,8,8);
    window.setNumSamples(8);
    window.setStencilBits(8);

    ofSetupOpenGL(&window,1920,1080,OF_FULLSCREEN);

    ofRunApp(new playerApp);
}
