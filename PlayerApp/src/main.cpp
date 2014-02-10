#include "ofMain.h"
#include "playerApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
<<<<<<< HEAD
    ofSetupOpenGL(&window,500,500,OF_WINDOW);//OF_FULLSCREEN);
=======
    ofSetupOpenGL(&window,1920,1080,OF_FULLSCREEN);//OF_FULLSCREEN);
>>>>>>> fd6ca34c1b1d7802d4d5924bdd08e19b5cec7693
    ofRunApp(new playerApp);

}
