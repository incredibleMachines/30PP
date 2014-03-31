#include "ofMain.h"
#include "testApp.h"

#include "ofxCocoa.h"
#include <Cocoa/Cocoa.h>



//========================================================================
int main()
{
	
	
	MSA::ofxCocoa::InitSettings			initSettings;
	
	initSettings.isOpaque				= true;
	initSettings.windowLevel			= NSMainMenuWindowLevel + 1;
    /* xcdoc://?url=developer.apple.com/library/mac/documentation/Cocoa/Reference/ApplicationKit/Classes/NSWindow_Class/Reference/Reference.html# */
    
	initSettings.hasWindowShadow		= false;
	initSettings.numFSAASamples			= true;
	initSettings.windowMode				= OF_FULLSCREEN;
	
	// to go fullscreen across all windows:
	initSettings.windowStyle			= NSBorderlessWindowMask;
    /* xcdoc://osx//library/mac/documentation/Cocoa/Reference/ApplicationKit/Classes/NSWindow_Class/Reference/Reference.html# */

//	initSettings.initRect				= MSA::ofxCocoa::rectForMainScreen();
	initSettings.initRect				= MSA::ofxCocoa::rectForAllScreens();
    NSLog(@"init width: %f", initSettings.initRect.size.width);
    NSLog(@"init height: %f", initSettings.initRect.size.height);
	
	MSA::ofxCocoa::setSyncToDisplayLink(true);
	MSA::ofxCocoa::AppWindow		cocoaWindow(initSettings);
	
    ofSetupOpenGL(&cocoaWindow,0,0,OF_FULLSCREEN);
	
	ofRunApp( new testApp() );
	
}