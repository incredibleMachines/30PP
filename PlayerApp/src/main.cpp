#include "ofMain.h"
#include "playerApp.h"
#include "ofAppGLFWWindow.h"
//#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGLFWWindow* win = new ofAppGLFWWindow;

    //GLFW GL SETUP
    //Don't seem to affect the FBO textures as these have their own GL settings but leaving them in for now as reference
    
    win->setMultiDisplayFullscreen(true);
//    win->setDoubleBuffering(true);
//    win->setVerticalSync(true);
//    win->setDepthBits(8);
//    win->setColorBits(8,8,8);
//    win->setNumSamples(8);
//    win->setStencilBits(8);
    
    win->setNumSamples(4);
    win->setOpenGLVersion(4,4);
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);

    ofSetupOpenGL(win,1920,1080,OF_FULLSCREEN);


    ofRunApp(new playerApp);
}
