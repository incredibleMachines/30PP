#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(70, 70, 70);
	ofEnableSmoothing();
	ofEnableDepthTest();
    
    cameraSelect=0;
    rotateSelect=false;
    scaleSelect=false;
    
    bool parsingSuccessful = settings.open("settings.json");
    
    if (parsingSuccessful) {
        cout<<"Number of Cameras: "+ofToString(settings["cameras"].size())<<endl;
        setupCameras();
	}
    
    else {
		cout  << "Failed to parse JSON" << endl;
	}
    
    ofDisableSeparateSpecularLight();
    
    bMouseDown=false;
    distThreshold=4;
    clickThreshold=10;
    
    adjustMode=ADJUST_MODE_CAMERA;

}

//--------------------------------------------------------------
void testApp::update(){
}


//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(255,255,255);
    
    if(adjustMode==ADJUST_MODE_CAMERA){
        ofDrawBitmapString("Adjust Camera Position using Arrows and z/a", 10, 10);
    }
    else if(adjustMode==ADJUST_MODE_LOOK_AT){
        ofDrawBitmapString("Adjust Camera Orientation using Arrows and z/a", 10, 10);
    }
    else if(adjustMode==ADJUST_MODE_VIEWPORT){
        ofDrawBitmapString("Adjust Camera Viewport Alignment using Arrows", 10, 10);
    }
    else if(adjustMode==ADJUST_MODE_MESH){
        ofDrawBitmapString("Adjust Mesh Points by Selecting with Mouse/Drag and using Arrows and z/a", 10, 10);
    }

	// draw viewports
	for(int i = 0; i < N_CAMERAS; i++){
        if(i==0){
            cameras[cameraSelect].camera.begin(cameras[0].viewport);
            drawScene(cameraSelect);
            cameras[cameraSelect].camera.end();
        }
        else{
            cameras[i].camera.begin(cameras[i].viewport);
            drawScene(i);
            cameras[i].camera.end();
        }
		
	}

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
    
    if(bMouseDown==true){
        ofRect(selectRect);
    }
	
    ofVec3f nearestVertex;
    int nearestIndex = 0;
	int n = cameras[cameraSelect].mesh.getNumVertices();
	float nearestDistance = clickThreshold;
    
	ofVec2f mouse(mouseX, mouseY);
	for(int i = 0; i < n; i++) {
		ofVec3f cur = cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(i));
		float distance = cur.distance(mouse);
		if(distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}
    
	ofNoFill();
    ofSetLineWidth(2);
    
    ofSetColor(ofColor::yellow);
    ofCircle(nearestVertex, 4);
    
    for(int i=0;i<moveVertices.size();i++){
        ofSetColor(255,0,0);
        if(moveVertices[i].drawHighlight==true){
            ofCircle(cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)).x+cameras[cameraSelect].viewport.x,cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)).y+cameras[cameraSelect].viewport.y,4);
            ofPushMatrix();
            ofTranslate(cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)).x+cameras[cameraSelect].viewport.x,cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)).y+cameras[cameraSelect].viewport.y);
            ofDrawAxis(20);
            ofPopMatrix();
        }
    }
    
	ofSetLineWidth(1);
}

void testApp::drawScene(int iCameraDraw){

    ofSetColor(ofColor::white);
	cameras[iCameraDraw].mesh.drawWireframe();



}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
      
    switch(key){
        case '0':
            cameraSelect=0;
            cameras[0].mesh=cameras[cameraSelect].mesh;
            break;
        case '1':
            cameraSelect=1;
            cameras[0].mesh=cameras[cameraSelect].mesh;
            break;
            
        case 'c':
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustMode=ADJUST_MODE_VIEWPORT;
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                adjustMode=ADJUST_MODE_LOOK_AT;
            }
            
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustMode=ADJUST_MODE_MESH;
            }
            else {
                adjustMode=ADJUST_MODE_CAMERA;
            }
            break;
            
        case OF_KEY_UP:
            if(adjustMode==ADJUST_MODE_CAMERA){
                cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(0,1,0));
                
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                ofQuaternion yRot(1, ofVec3f(0,1,0));
                ofQuaternion xRot(0, ofVec3f(1,0,0));
                ofQuaternion zRot(0, ofVec3f(0,0,1));
                cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.y--;
                
            }
            else{
                for(int i=0;i<moveVertices.size();i++){
                    cameras[cameraSelect].mesh.setVertex(moveVertices[i].index,cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)+ofVec3f(0,.1,0));
                }
            }
            break;
            
        case OF_KEY_DOWN:
            if(adjustMode==ADJUST_MODE_CAMERA){
                cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(0,-1,0));
                
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                    ofQuaternion yRot(-1, ofVec3f(0,1,0));
                    ofQuaternion xRot(0, ofVec3f(1,0,0));
                    ofQuaternion zRot(0, ofVec3f(0,0,1));
                    cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.y++;
                
            }
            else{
                for(int i=0;i<moveVertices.size();i++){
                    cameras[cameraSelect].mesh.setVertex(moveVertices[i].index,cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)+ofVec3f(0,-.1,0));
                }
            }
            break;
            
        case OF_KEY_RIGHT:
            if(adjustMode==ADJUST_MODE_CAMERA){
                cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(1,0,0));
                
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                ofQuaternion yRot(0, ofVec3f(0,1,0));
                ofQuaternion xRot(1, ofVec3f(1,0,0));
                ofQuaternion zRot(0, ofVec3f(0,0,1));
                cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.x++;
                
            }
            else{
                for(int i=0;i<moveVertices.size();i++){
                    cameras[cameraSelect].mesh.setVertex(moveVertices[i].index,cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)+ofVec3f(.1,0,0));
                }
            }
            break;
        case OF_KEY_LEFT:
            if(adjustMode==ADJUST_MODE_CAMERA){
                cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(-1,0,0));
                
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                ofQuaternion yRot(0, ofVec3f(0,1,0));
                ofQuaternion xRot(-1, ofVec3f(1,0,0));
                ofQuaternion zRot(0, ofVec3f(0,0,1));
                cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.x--;
                
            }
            else{
            for(int i=0;i<moveVertices.size();i++){
                cameras[cameraSelect].mesh.setVertex(moveVertices[i].index,cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)+ofVec3f(-.1,0,0));
            }
            }
            break;
        case 'z':
            if(adjustMode==ADJUST_MODE_CAMERA){
                cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(0,0,-1));
                
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                ofQuaternion yRot(0, ofVec3f(0,1,0));
                ofQuaternion xRot(0, ofVec3f(1,0,0));
                ofQuaternion zRot(-1, ofVec3f(0,0,1));
                cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
            }
            else{
                for(int i=0;i<moveVertices.size();i++){
                    cameras[i].mesh.setVertex(moveVertices[i].index,cameras[i].mesh.getVertex(moveVertices[i].index)-ofVec3f(0,0,-.1));
                }
            }
            break;
        case 'a':
            if(adjustMode==ADJUST_MODE_CAMERA){
                cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(0,0,1));
                
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                ofQuaternion yRot(0, ofVec3f(0,1,0));
                ofQuaternion xRot(0, ofVec3f(0,0,0));
                ofQuaternion zRot(1, ofVec3f(0,0,1));
                cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
            }
            else{
                for(int i=0;i<moveVertices.size();i++){
                    cameras[cameraSelect].mesh.setVertex(moveVertices[i].index,cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)+ofVec3f(0,0,-.1));
                }
            }
            break;
        case 's':
            cameras[cameraSelect].mesh.save("test.ply");
            saveCameras();
            break;
        case 'r':
            rotateSelect=true;
            break;
        case 't':
            scaleSelect=true;
            break;
        case 'm':
            ofxAssimpModelLoader reload;
            reload.loadModel("3wall/3walls.dae");
            cameras[cameraSelect].mesh=reload.getCurrentAnimatedMesh(0);
            cout<<"Reloaded Model"<<endl;
            break;
            
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    switch(key){
        case 'r':
            rotateSelect=false;
            break;
        case 't':
            scaleSelect=false;
            break;
            
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    moveVertices.clear();
    selectRect=ofRectangle(mouseDown,ofVec2f(mouseX,mouseY));
    int n = cameras[cameraSelect].mesh.getNumVertices();
	ofVec2f mouse(mouseX, mouseY);
	for(int i = 0; i < n; i++) {
		ofVec3f cur = cameras[0].camera.worldToScreen(cameras[0].mesh.getVertex(i));
		if(selectRect.inside(cur)) {
            meshVertex tempVert;
            tempVert.vertex=cur;
            tempVert.index=i;
            moveVertices.push_back(tempVert);
        }
    }
    for(int i=0;i<moveVertices.size();i++){
        if(i==0){
            moveVertices[i].drawHighlight=true;
        }
        else{
            for (int j=0;j<moveVertices.size();j++){
                float distance=moveVertices[i].vertex.distance(moveVertices[j].vertex);
                if(distance>4){
                    moveVertices[i].drawHighlight=true;
                    break;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(bMouseDown==false){
        mouseDown=ofVec2f(mouseX,mouseY);
        bMouseDown=true;
    }
    moveVertices.clear();
    int n = cameras[0].mesh.getNumVertices();
    int nearestDistance=distThreshold;
	
	ofVec2f mouse(mouseX, mouseY);
	for(int i = 0; i < n; i++) {
		ofVec3f cur = cameras[0].camera.worldToScreen(cameras[0].mesh.getVertex(i));
		float distance = cur.distance(mouse);
		if(distance < nearestDistance) {
            meshVertex tempVert;
            tempVert.vertex=cur;
            tempVert.index=i;
            moveVertices.push_back(tempVert);
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    bMouseDown=false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

void testApp:: setupCameras() {
    for(int i=0; i<N_CAMERAS;i++){
        if (settings["cameras"].size()>i){
            ofMesh mesh;
            if(i==0){
                mesh.load("mesh_1.ply");
            }
            mesh.load("mesh_"+ofToString(i)+".ply");
            ofVec3f pos = ofVec3f(settings["cameras"][i]["pos"]["x"].asFloat(), settings["cameras"][i]["pos"]["y"].asFloat(), settings["cameras"][i]["pos"]["z"].asFloat());
            ofQuaternion rot = ofQuaternion(settings["cameras"][i]["orientation"]["x"].asFloat(), settings["cameras"][i]["orientation"]["y"].asFloat(), settings["cameras"][i]["orientation"]["z"].asFloat(), settings["cameras"][i]["orientation"]["w"].asFloat());
            ofVec3f viewPos = ofVec3f(settings["cameras"][i]["viewPos"]["x"].asFloat(),settings["cameras"][i]["viewPos"]["y"].asFloat());
            ofVec3f viewSize = ofVec3f(settings["cameras"][i]["viewSize"]["x"].asFloat(),settings["cameras"][i]["viewSize"]["y"].asFloat());
            cameras[i].setup(pos, rot, viewPos,viewSize,mesh);
        }
    }
}

void testApp:: saveCameras() {
    for(int i=0; i<N_CAMERAS;i++){
        settings["cameras"][i]["pos"]["x"]=cameras[i].camera.getGlobalPosition().x;
        settings["cameras"][i]["pos"]["y"]=cameras[i].camera.getGlobalPosition().y;
        settings["cameras"][i]["pos"]["z"]=cameras[i].camera.getGlobalPosition().z;
        settings["cameras"][i]["viewPos"]["x"]=cameras[i].viewport.x;
        settings["cameras"][i]["viewPos"]["y"]=cameras[i].viewport.y;
        settings["cameras"][i]["orientation"]["x"]=cameras[i].camera.getGlobalOrientation().x();
        settings["cameras"][i]["orientation"]["y"]=cameras[i].camera.getGlobalOrientation().y();
        settings["cameras"][i]["orientation"]["z"]=cameras[i].camera.getGlobalOrientation().z();
        settings["cameras"][i]["orientation"]["w"]=cameras[i].camera.getGlobalOrientation().w();
    }
    settings.save("settings.json");
}


