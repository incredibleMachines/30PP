#include "ofMain.h"
#include "testApp.h"

#include "ofxCocoa.h"
#include <Cocoa/Cocoa.h>



//========================================================================
int main()
{
	MSA::ofxCocoa::InitSettings			initSettings;
	
    //INIT SETTINGS FOR OFXCOCOA WINDOW
    
	initSettings.isOpaque				= true;
	initSettings.windowLevel			= NSMainMenuWindowLevel + 1;
    /* xcdoc://?url=developer.apple.com/library/mac/documentation/Cocoa/Reference/ApplicationKit/Classes/NSWindow_Class/Reference/Reference.html# */
	initSettings.hasWindowShadow		= false;
	initSettings.numFSAASamples			= true;
	initSettings.windowMode				= OF_FULLSCREEN;
	// to go fullscreen across all windows:
	initSettings.windowStyle			= NSBorderlessWindowMask;
    /* xcdoc://osx//library/mac/documentation/Cocoa/Reference/ApplicationKit/Classes/NSWindow_Class/Reference/Reference.html# */
	initSettings.initRect				= MSA::ofxCocoa::rectForAllScreens();
	
    //CREATE GL CONTEXT
	MSA::ofxCocoa::setSyncToDisplayLink(true);
	MSA::ofxCocoa::AppWindow		cocoaWindow(initSettings);
    ofSetupOpenGL(&cocoaWindow,0,0,OF_FULLSCREEN);
	
	ofRunApp( new testApp() );
	
}
