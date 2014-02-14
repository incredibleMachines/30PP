#include "ofMain.h"
#include "playerApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

//        ofEnableAntiAliasing();
    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    window.setDoubleBuffering(true);
    window.setVerticalSync(true);
    window.setDepthBits(8);
    window.setColorBits(8,8,8);
    window.setNumSamples(8);
    window.setStencilBits(8);
    window.setOpenGLVersion(GL_VERSION_4_1, GL_VERSION_3_2);
//    ofSetupOpenGL(&window,500,500,OF_WINDOW);
    ofSetupOpenGL(&window,1920,1080,OF_FULLSCREEN);

//    	ofSetVerticalSync(true);
    ofRunApp(new playerApp);

}
