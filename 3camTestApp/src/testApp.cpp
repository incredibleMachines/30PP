



#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    
    ofBackground(0, 0, 0);
    
    cameraSelect=1;
    
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
    clickThreshold=4;
    
    adjustMode=ADJUST_MODE_CAMERA;
    
    player.setPixelFormat(OF_PIXELS_RGBA);

	ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
    player.loadMovie("Bonus_waves.mov", decodeMode);
    player.play();

}

//--------------------------------------------------------------
void testApp::update(){
    //update video texture
    player.update();
    
    //update gui camera to display selected camera
    cameras[GUI_CAMERA].mesh=cameras[cameraSelect].mesh;
    cameras[GUI_CAMERA].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation());
    cameras[GUI_CAMERA].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition());
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
    //DRAW GUI
    
    ofPushStyle();
	glDepthFunc(GL_ALWAYS);
    drawGuiText();
    glDepthFunc(GL_LESS);
	ofPopStyle();
    
    //DRAW CAMERAS, MESHES AND TEXTURES
    
    drawCameras();
    
    //DRAW POINT SELECTION HIGHLIGHTS
    
    drawHighlights();
    
   
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
      
    switch(key){
            
        case '1':
            cameraSelect=1;
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
                    cameras[cameraSelect].mesh.setVertex(moveVertices[i].index,cameras[cameraSelect].mesh.getVertex(moveVertices[i].index)-ofVec3f(0,0,-.1));
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
            saveCameras();
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

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    //clear selected vertex vector
    moveVertices.clear();
    
    //set up select rectangle
    selectRect=ofRectangle(mouseDown,ofVec2f(mouseX,mouseY));
    
    //determine vertices inside drag box
    int n = cameras[cameraSelect].mesh.getNumVertices();
	for(int i = 0; i < n; i++){
        ofVec3f cur = cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(i),cameras[cameraSelect].viewport);
		if(selectRect.inside(cur)) {
            meshVertex tempVert;
            tempVert.vertex=cur;
            tempVert.index=i;
            moveVertices.push_back(tempVert);
        }
    }
    
    //determine vertices inside drag box on gui camera screen
    for(int i = 0; i < n; i++){
        ofVec3f cur = cameras[GUI_CAMERA].camera.worldToScreen(cameras[GUI_CAMERA].mesh.getVertex(i),cameras[GUI_CAMERA].viewport);
		if(selectRect.inside(cur)) {
            meshVertex tempVert;
            tempVert.vertex=cur;
            tempVert.index=i;
            moveVertices.push_back(tempVert);
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //determine whether to begin drawing draw box for mouseDragged()
    if(bMouseDown==false){
        selectRect=ofRectangle(0,0,0,0);
        mouseDown=ofVec2f(mouseX,mouseY);
        bMouseDown=true;
    }
    
    //clear selected vertex vector
    moveVertices.clear();
    
    //check for click inside click threshold, if present highlight this vertex individually 
    int n = cameras[GUI_CAMERA].mesh.getNumVertices();
    int nearestDistance=clickThreshold;
	for(int i = 0; i < n; i++) {
        ofVec3f cur = cameras[GUI_CAMERA].camera.worldToScreen(cameras[GUI_CAMERA].mesh.getVertex(i),cameras[GUI_CAMERA].viewport);
		float distance = cur.distance(ofVec2f(mouseX,mouseY));
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
    //reset drag box status
    bMouseDown=false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

void testApp:: setupCameras() {
    for(int i=0; i<N_CAMERAS;i++){
        if (settings["cameras"].size()>i){
            ofMesh mesh;
            string loader="mesh_"+ofToString(i)+".ply";
            mesh.load(loader);
            int n = mesh.getNumVertices();
            for(int i=0;i<n;i++){
                mesh.addTexCoord(ofVec3f(0,0,0));
                mesh.addTexCoord(ofVec3f(1,0,0));
                mesh.addTexCoord(ofVec3f(1,1,0));
                mesh.addTexCoord(ofVec3f(0,1,0));
            }
            ofVec3f pos = ofVec3f(settings["cameras"][i]["pos"]["x"].asFloat(), settings["cameras"][i]["pos"]["y"].asFloat(), settings["cameras"][i]["pos"]["z"].asFloat());
            ofQuaternion rot = ofQuaternion(settings["cameras"][i]["orientation"]["x"].asFloat(), settings["cameras"][i]["orientation"]["y"].asFloat(), settings["cameras"][i]["orientation"]["z"].asFloat(), settings["cameras"][i]["orientation"]["w"].asFloat());
            ofVec3f viewPos = ofVec3f(settings["cameras"][i]["viewPos"]["x"].asFloat(),settings["cameras"][i]["viewPos"]["y"].asFloat());
            ofVec3f viewSize = ofVec3f(settings["cameras"][i]["viewSize"]["x"].asFloat(),settings["cameras"][i]["viewSize"]["y"].asFloat());
            cameras[i].setup(pos, rot, viewPos, viewSize, mesh);
        }
    }
}

void testApp:: saveCameras() {
    for(int i=0; i<N_CAMERAS;i++){
        
        //SAVE CAMERA DATA
        
        //Camera position
        settings["cameras"][i]["pos"]["x"]=cameras[i].camera.getGlobalPosition().x;
        settings["cameras"][i]["pos"]["y"]=cameras[i].camera.getGlobalPosition().y;
        settings["cameras"][i]["pos"]["z"]=cameras[i].camera.getGlobalPosition().z;
        
        //camera viewport offset
        settings["cameras"][i]["viewPos"]["x"]=cameras[i].viewport.x;
        settings["cameras"][i]["viewPos"]["y"]=cameras[i].viewport.y;
        
        //camera orientation quaternion
        settings["cameras"][i]["orientation"]["x"]=cameras[i].camera.getGlobalOrientation().x();
        settings["cameras"][i]["orientation"]["y"]=cameras[i].camera.getGlobalOrientation().y();
        settings["cameras"][i]["orientation"]["z"]=cameras[i].camera.getGlobalOrientation().z();
        settings["cameras"][i]["orientation"]["w"]=cameras[i].camera.getGlobalOrientation().w();
        
        //SAVE INDIVIDUAL MESH
        
        //save warped mesh object
        string meshname;
        meshname="mesh_"+ofToString(i)+".ply";
        cameras[i].mesh.save(meshname);
    }
    
    //SAVE JSON SETTINGS DOCUMENT
    
    settings.save("settings.json");
}

void testApp:: drawGuiText() {
    
    //Check adjustMode and apply relevant text/instructions
    string cameraData="Hit c to Change Adjust Mode. Current Mode: ";
    if(adjustMode==ADJUST_MODE_CAMERA){
        cameraData+="Adjust Camera Position using Arrows and z/a";
    }
    else if(adjustMode==ADJUST_MODE_LOOK_AT){
        cameraData+="Adjust Camera Orientation using Arrows and z/a";
    }
    else if(adjustMode==ADJUST_MODE_VIEWPORT){
        cameraData+="Adjust Camera Viewport Alignment using Arrows";
    }
    else if(adjustMode==ADJUST_MODE_MESH){
        cameraData+="Adjust Mesh Points by Selecting with Mouse/Drag and using Arrows and z/a";
    }
    
    //DRAW GUI TEXT
    
    ofSetColor(255,255,255);
    
    //Draw selected camera number
    ofDrawBitmapString("Adjusting Camera "+ofToString(cameraSelect), 10, 20);
    
    //Draw current adjust mode and instructions
    ofDrawBitmapString(cameraData, cameras[GUI_CAMERA].viewport.x+10, cameras[GUI_CAMERA].viewport.y+50);
    
    //Draw framerate
    ofDrawBitmapString("Framerate"+ofToString(ofGetFrameRate()), cameras[GUI_CAMERA].viewport.x+10, cameras[GUI_CAMERA].viewport.y+80);
}

void testApp:: drawCameras() {
    for(int i = 0; i < N_CAMERAS; i++){
        
        //CREATE AND POPULATE CAMERA
        
        //Begin camera object
        cameras[i].camera.begin(cameras[i].viewport);
        
        //bind video texture to mesh
        player.getTexture()->bind();
        
        //draw mesh
        ofSetColor(255,255,255);
        cameras[i].mesh.drawFaces();
        
        //unbind video texture
        player.getTexture()->unbind();
       
        //DRAW MESH WIREFRAME
        ofSetColor(0,0,0);
        ofSetLineWidth(2);
        cameras[i].mesh.drawWireframe();
        
        //end camera object
        cameras[i].camera.end();
    }
}

void testApp::drawHighlights() {

    //DRAW SELECT BOX
    
    //draw drag and select box, set in mouseDragged()
    if(bMouseDown==true){
        ofSetColor(255,255,255);
        ofSetLineWidth(1);
        ofRect(selectRect);
    }
    
    //DRAW NEAREST VERTEX
    
    //determine nearest vertex and highlight yellow, if within clickThreshold distance of vertex

    //set up local variables for determining nearest
    ofVec3f nearestVertex;
    int nearestIndex = 0;
	int n = cameras[cameraSelect].mesh.getNumVertices();
	float nearestDistance = clickThreshold;
    float drawNearest=false;
	ofVec2f mouse(mouseX, mouseY);
    
    //parse through all vertices to determine nearest, if exists
	for(int i = 0; i < n; i++) {
		ofVec3f cur = cameras[GUI_CAMERA].camera.worldToScreen(cameras[GUI_CAMERA].mesh.getVertex(i),cameras[GUI_CAMERA].viewport);
		float distance = cur.distance(mouse);
		if(distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
            drawNearest=true;
		}
	}
    
    //draw nearest highlight circle on both gui camera and selected camera
	ofNoFill();
    ofSetLineWidth(2);
    if(drawNearest==true){
        ofSetColor(ofColor::yellow);
        ofCircle(nearestVertex, 4);
        ofCircle(cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(nearestIndex),cameras[cameraSelect].viewport),4);
    }
    
    //DRAW SELECTED VERTICES
    
    //draw selected circles on all vertices selected by clicking or via drag box on both gui camera and selected camera, set in mousePressed() and mouseDragged()
    ofSetColor(255,0,0);
    for(int i=0;i<moveVertices.size();i++){
        ofPushMatrix();
        ofVec3f translate=cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh.getVertex(moveVertices[i].index),cameras[cameraSelect].viewport);;
        ofTranslate(translate);
        ofCircle(0,0,4);
        ofDrawAxis(20);
        ofPopMatrix();
        
        if(cameraSelect!=GUI_CAMERA){
            ofPushMatrix();
            ofVec3f translate=cameras[GUI_CAMERA].camera.worldToScreen(cameras[GUI_CAMERA].mesh.getVertex(moveVertices[i].index),cameras[GUI_CAMERA].viewport);;
            ofTranslate(translate);
            ofCircle(0,0,4);
            ofDrawAxis(20);
            ofPopMatrix();
        }
    }
    
	ofSetLineWidth(1);
}


