#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(70, 70, 70);
	ofEnableSmoothing();
	ofEnableDepthTest();
    
    bool parsingSuccessful = settings.open("settings.json");
    
    if (parsingSuccessful) {
        
        //--
        // Setup cameras
        
        setupCameras();
		
	}
    
    else {
		cout  << "Failed to parse JSON" << endl;
	}


    
    model.loadModel("squirrel/NewSquirrel.3ds");
    model.setPosition(0, 0, 0);
    model.setScale(.02,.02,.02);
    ofDisableSeparateSpecularLight();

}

//--------------------------------------------------------------
void testApp::update(){

}


//--------------------------------------------------------------
void testApp::draw(){

	ofDrawBitmapString("test", 10, 10);




	//--
	// Draw all viewports


	// draw side viewports
	for(int i = 0; i < N_CAMERAS; i++){
		cameras[i].camera.begin(cameras[i].viewport);
		drawScene(i);
		cameras[i].camera.end();
	}

	//
	//--



	//--
	// Draw annotations (text, gui, etc)

	ofPushStyle();
	glDepthFunc(GL_ALWAYS); // draw on top of everything

	// draw some labels
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("framerate"+ofToString(ofGetFrameRate()), 20, 30);



	// draw outlines on views
	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255, 255, 255);
	//
	for(int i = 0; i < N_CAMERAS; i++){
		ofRect(cameras[i].viewport);
	}
	//


	// restore the GL depth function
	glDepthFunc(GL_LESS);
	ofPopStyle();

	//
	//--
}

void testApp::drawScene(int iCameraDraw){

	nodeGrid.draw();
    model.drawFaces();

	//--
	// Draw frustum preview for ofEasyCam camera

	// This code draws our camera in
	//	the scene (reddy/pink lines)
	//
	// The pyramid-like shape defined
	//	by the cameras view is called
	//	a 'frustum'.
	//
	// Often we refer to the volume
	//	which can be seen by the
	//	camera as 'the view frustum'.



		ofPushStyle();
		ofPushMatrix();

		//--
		// Create transform for box->frustum

		// In 'camera space' the bounds of
		//  the view are defined by a box
		//  with bounds -1->1 in each axis
		//
		// To convert from camera to world
		//  space, we multiply by the inverse
		//  camera matrix of the camera, i.e
		//  inverse of the ViewProjection
		//  matrix.
		//
		// By applying this transformation
		//  our box in camera space is
		//  transformed into a frustum in
		//  world space.
		//

		// The camera's matricies are dependant on
		//  the aspect ratio of the viewport.
		//  (Which is why we use the viewport as
		//  an argument when we begin the camera.
		//
		// If this camera is fullscreen we'll use
		//   viewMain, else we'll use viewGrid[0]
		ofRectangle boundsToUse;

        boundsToUse = cameras[iCameraDraw].viewport;


		// Now lets get the inverse ViewProjection
		//  for the camera
		ofMatrix4x4 inverseCameraMatrix;
		inverseCameraMatrix.makeInvertOf(cameras[iCameraDraw].camera.getModelViewProjectionMatrix(boundsToUse));

		// By default, we can say
		//	'we are drawing in world space'
		//
		// The camera matrix performs
		//	world->camera
		//
		// The inverse camera matrix performs
		//	camera->world
		//
		// Our box is in camera space, if we
		//	want to draw that into world space
		//	we have to apply the camera->world
		//	transformation.
		//

		// This syntax is a little messy.
		// What it's saying is, send the data
		//  from the inverseCameraMatrix object
		//  to OpenGL, and apply that matrix to
		//  the current OpenGL transform
		ofMultMatrix( inverseCameraMatrix );

		//
		//--



		//--
		// Draw box in camera space
		// (i.e. frustum in world space)

		ofNoFill();
		// i.e. a box -1, -1, -1 to +1, +1, +1
		ofDrawBox(0, 0, 0, 2.0f);
		//
		//--

		ofPopStyle();
		ofPopMatrix();

	//
	//--


}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
//TODO: a-z zoom in/out, arrows x/y, num keys select camera
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

void testApp:: setupCameras() {
    for(int i=0; i<N_CAMERAS;i++){
        if (settings["cameras"].size()>i){
            ofVec3f camPos = ofVec3f(settings["cameras"][i]["camPos"]["x"].asFloat(), settings["cameras"][i]["camPos"]["y"].asFloat(), settings["cameras"][i]["camPos"]["z"].asFloat());
            ofVec3f lookPos = ofVec3f(settings["cameras"][i]["lookPos"]["x"].asFloat(),settings["cameras"][i]["lookPos"]["y"].asFloat(),settings["cameras"][i]["lookPos"]["z"].asFloat());
            ofVec3f viewPos = ofVec3f(settings["cameras"][i]["viewPos"]["x"].asFloat(),settings["cameras"][i]["viewPos"]["y"].asFloat());
            ofVec3f viewSize = ofVec3f(settings["cameras"][i]["viewSize"]["x"].asFloat(),settings["cameras"][i]["viewSize"]["y"].asFloat());
            cameras[i].setup(camPos, lookPos, viewPos,viewSize);
        }
    }
}

