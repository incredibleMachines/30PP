#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(70, 70, 70);
	ofEnableSmoothing();
	ofEnableDepthTest();


	//--
	// Setup cameras

	cam1.scale = 20;
    cam1.tilt(-90);
	cameras[0] = &cam1;
    
    cam2.scale = 20;
	cameras[1] = &cam2;
    
    cam3.scale=20;
    cam3.pan(90);
    cameras[2]= &cam3;




	//--
	// Define viewports

	setupViewports();

	//
	//--




	//--
	// Setup swarm

	// swarm is a custom ofNode in this example (see Swarm.h / Swarm.cpp)
	nodeSwarm.init(100, 50, 10);

	//
	//--
}

//--------------------------------------------------------------
void testApp::setupViewports(){
	//call here whenever we resize the window


	//--
	// Define viewports

    viewGrid[0].x=0;
    viewGrid[0].y=0;
    viewGrid[0].width=1440;
    viewGrid[0].height=900;
    
    viewGrid[1].x=1440;
    viewGrid[1].y=0;
    viewGrid[1].width=1920;
    viewGrid[1].height=1080;
   
    
    viewGrid[2].x=3360;
    viewGrid[2].y=0;
    viewGrid[2].width=1920;
    viewGrid[2].height=1080;
    

	//
	//--
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
		cameras[i]->begin(viewGrid[i]);
		drawScene(i);
		cameras[i]->end();
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
		ofRect(viewGrid[i]);
	}
	//


	// restore the GL depth function
	glDepthFunc(GL_LESS);
	ofPopStyle();

	//
	//--
}

void testApp::drawScene(int iCameraDraw){

	nodeSwarm.draw();
	nodeGrid.draw();

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


	// First check if we're already drawing the view through the easycam
	// If so, don't draw the frustum preview.
	if(iCameraDraw != 0){

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

        boundsToUse = viewGrid[0];


		// Now lets get the inverse ViewProjection
		//  for the camera
		ofMatrix4x4 inverseCameraMatrix;
		inverseCameraMatrix.makeInvertOf(cam1.getModelViewProjectionMatrix(boundsToUse));

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
	}

	//
	//--


}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
	setupViewports();
}

