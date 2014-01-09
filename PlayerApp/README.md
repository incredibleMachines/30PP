Instructions for PlayerApp
-----------------------

- Assumptions: You have openframeworks 0.8.0 installed.

Install ofxSyphon:
-  add ofxSyphon to your addons folder
-  add ofxSyphon/libs/Syphon/lib/osx/Syphon.framework to the 'Copy Files' under 'Build Phases'

Install ofxLibwebsockets:
-  $ cd /path/to/your/OF/Folder/{addons}
-  $ git clone [https://github.com/labatrockwell/ofxLibwebsockets.git] (https://github.com/labatrockwell/ofxLibwebsockets.git)
	
	FOR OSX DELETE
		- Delete ofxLibwebsockets/libs/libwebsockets/include/win32port
	
###To Run:###
-  execute ControllerApp in terminal
-  run this PlayerApp from xcode
-  press spacebar to request an init command from ControllerApp.
	
***Currently all video output (syphon class) is commented due to bug with file path/directory in Asset.cpp as well as ControllerApp path naming