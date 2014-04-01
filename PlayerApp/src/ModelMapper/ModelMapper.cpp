//
//  ModelMapper.cpp
//  3camTest
//
//  Created by IncredibleMachines on 2/5/14.
//
//

/*
 ModelMapper class. This class loads meshes into user-defined number of cameras. Each camera's position and orientation can be set individually, as can all vertices on each cameras meshes. Masks can be added to each camera. All gui text drawn in this class.
 
 methods
 - setup() pass mesh and camera data into ModelMapper. add listeners for key and mouse presses. create texture vetex with blank data
 - setupCameras() load camera and mask data from JSON settings file. Create Camera objects.
 - update() update textures and set guicam to be identical to selected camera
 - draw() trigger various drawing functions
 - drawGuiText() draw gui text, dependent on ADJUST_MODE
 - drawCameras() wrap meshes in their designated textures and draw out to all monitors
 - drawHighlights() draw on-screen highlights for selected mesh and mask vertices
 - drawMasks() draw ofPath for all masks
 - saveCameras() save camera data to JSON settings file and mesh data to ply files
 -setReloadMesh(string) pass in filepath to dae or obj file for reset mesh
 */

#include "ModelMapper.h"

void ModelMapper::setup(int _numCams){
    
    //pass through defaults
    setup(_numCams,0,1);
}

void ModelMapper::setup(int _numCams, int _guiCam){
    
    //pass through with defaults
    setup(_numCams,_guiCam,1);
}

void ModelMapper::setup(int _numCams, int _guiCam, int _numMeshes){
    
    //pass through with defaults
    vector<int> _whichMeshes;
    _whichMeshes.push_back(0);
    _whichMeshes.push_back(1);
    _whichMeshes.push_back(2);
    setup(_numCams,_guiCam,_whichMeshes);
    
    cout<<"setup"<<endl;
}

void ModelMapper::setup(int _numCams, int _guiCam, vector<int> _whichMeshes){
    
    
    //----------SETUP GLOBALS

    numCams=_numCams;
    guiCam=_guiCam;
    numMeshes=_whichMeshes.size();
    whichMeshes=_whichMeshes;
    
    //set default variable values
    //camera settings
    cameraSelect=1;
    adjustMode=ADJUST_MODE_CAMERA;
    meshType=MESH_MASS;
    magnetMode=MAGNET_MODE_NONE;
    
    //mouse settings
    bMouseDown=false;
    clickThreshold=2;
    
    //for checking for double click on mask creation
    mouseTimer=ofGetElapsedTimeMillis();
    
    //mask settings
    bNewMask=false;
    bDrawingMask=false;
    bMaskPoint=false;
    
    //draw settings
    bDrawGui=true;
    bDrawWireframe=false;
    
    //adjustment settings
    bShiftPressed=false;
    moveModifier=1;
    
    //texture settings
    bMipMap=true;
    
    //----------LOAD SETTINGS JSON
    
    ofBuffer buffer = ofBufferFromFile("settings.json");
    
    cout<< "buffer.getText.length(): "<< buffer.getText().length() <<endl;
    
    Json::Reader reader;
    if(reader.parse(buffer.getText(), settings, false)){
        cout<<"Number of Cameras: "+ofToString(settings["cameras"].size())<<endl;
        setupCameras();
    } else {
        cout  << "Failed to parse JSON: " <<  reader.getFormatedErrorMessages() << endl;
	}
    
    //----------SETUP LISTENERS
    
    //Event Listeners for key and mouse events
    ofAddListener(ofEvents().keyPressed,this,&ModelMapper::keyPressed);
    ofAddListener(ofEvents().keyReleased,this,&ModelMapper::keyReleased);
    ofAddListener(ofEvents().mousePressed,this,&ModelMapper::mousePressed);
    ofAddListener(ofEvents().mouseDragged,this,&ModelMapper::mouseDragged);
    ofAddListener(ofEvents().mouseReleased,this,&ModelMapper::mouseReleased);
    ofAddListener(ofEvents().mouseMoved,this,&ModelMapper::mouseMoved);
    
}

void ModelMapper::update(vector<ofTexture *> tex){
    
    //update gui camera to display selected camera graphics
    textures=tex;
    for(int i=0;i<numMeshes;i++){
        cameras[guiCam].mesh[i]=cameras[cameraSelect].mesh[i];
    }
    cameras[guiCam].masks=cameras[cameraSelect].masks;
    cameras[guiCam].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation());
    cameras[guiCam].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition());
    cameras[guiCam].highlightMask=cameras[cameraSelect].highlightMask;
}

void ModelMapper::draw(){
    
    //----------DRAW CAMERAS, MESHES, TEXTURES
    drawCameras();
    
    //----------DRAW MESH POINT SELECTION HIGHLIGHTS
    ofPushStyle();
	glDepthFunc(GL_ALWAYS);
    drawHighlights();
    glDepthFunc(GL_LESS);
	ofPopStyle();
    
    //----------DRAW MASKS
    drawMasks();
    
    //----------DRAW GUI
    if(bDrawGui==true){
        ofPushStyle();
        glDepthFunc(GL_ALWAYS);
        drawGuiText();
        glDepthFunc(GL_LESS);
        ofPopStyle();
    }
    
}

void ModelMapper::keyPressed(ofKeyEventArgs& args){
    
    //set moveModifier to default if no modifier pressed
    if(bShiftPressed==false&&bCommandPressed==false){
        moveModifier=1;
    }
    
    switch(args.key){
            
        //----------SELECT ACTIVE CAMERA

        case '1':
            cameraSelect=1;
            updateMasks();
            break;
        case '2':
            cameraSelect=2;
            updateMasks();
            break;
        case '3':
            cameraSelect=3;
            updateMasks();
            break;
            
        //----------MODIFY ADJUSTMENTS
        
        case OF_KEY_COMMAND:
            bCommandPressed=true;
            moveModifier=.1;
            break;
        
        case OF_KEY_SHIFT:
            bShiftPressed=true;
            moveModifier=10;
            break;
            
        //----------SELECT ADJUST MODE
            
        case 'c':
            if(adjustMode!=ADJUST_MODE_LOCKED){
                bDrawGui=true;
            }
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustMode=ADJUST_MODE_VIEWPORT;
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                adjustMode=ADJUST_MODE_LOOK_AT;
            }
            
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustMode=ADJUST_MODE_MESH;
            }
            else if(adjustMode==ADJUST_MODE_MESH){
                adjustMode=ADJUST_MODE_MASK;
            }
            else if (adjustMode==ADJUST_MODE_MASK){
                adjustMode=ADJUST_MODE_CAMERA;
            }
            break;
            
            //----------MAKE ADJUSTMENTS
            
        case OF_KEY_UP:
            //Move Camera Position
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustPosition(0,moveModifier,0);
            }
            
            //Adjust Camera Orientation
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustOrientation(0,moveModifier/10,0);
            }
            
            //Adjust Viewport Position for Active Camera
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.y-=moveModifier;
            }
            
            //Adjust Entire Mask Position
            else if(adjustMode==ADJUST_MODE_MASK){
                adjustMask(0,-moveModifier);
            }
            
            //Adjust Mesh Point Positions
            else if(adjustMode==ADJUST_MODE_MESH){
                adjustMesh(0,moveModifier,0);
            }
            break;
            
        case OF_KEY_DOWN:
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustPosition(0,-moveModifier,0);
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustOrientation(0,-moveModifier/10,0);
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.y+=moveModifier;
            }
            else if(adjustMode==ADJUST_MODE_MASK){
                adjustMask(0,moveModifier);
            }
            else if(adjustMode==ADJUST_MODE_MESH){
                adjustMesh(0,-moveModifier,0);
            }
            break;
            
        case OF_KEY_RIGHT:
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustPosition(-moveModifier,0,0);
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustOrientation(-moveModifier/10,0,0);
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.x+=moveModifier;
                
            }
            else if(adjustMode==ADJUST_MODE_MASK){
                adjustMask(moveModifier,0);
            }
            else if(adjustMode==ADJUST_MODE_MESH){
                adjustMesh(-moveModifier,0,0);
            }
            break;
            
        case OF_KEY_LEFT:
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustPosition(moveModifier,0,0);
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustOrientation(moveModifier/10,0,0);
            }
            else if(adjustMode==ADJUST_MODE_VIEWPORT){
                cameras[cameraSelect].viewport.x-=moveModifier;
            }
            else if(adjustMode==ADJUST_MODE_MASK){
                adjustMask(-moveModifier,0);
            }
            else if(adjustMode==ADJUST_MODE_MESH){
                adjustMesh(moveModifier,0,0);
            }
            break;
            
        //A/Z Adjust Camera Z-Position, Z-Orientation and Mesh Z-Position
        case 'z':
        case 'Z':
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustPosition(0,0,moveModifier);
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustOrientation(0,0,-moveModifier/10);
            }
            else if(adjustMode==ADJUST_MODE_MESH){
                adjustMesh(0,0,moveModifier);
            }
            break;
            
        case 'a':
        case 'A':
            if(adjustMode==ADJUST_MODE_CAMERA){
                adjustPosition(0,0,-moveModifier);
            }
            else if(adjustMode==ADJUST_MODE_LOOK_AT){
                adjustOrientation(0,0,moveModifier/10);
            }
            else if(adjustMode==ADJUST_MODE_MESH){
                adjustMesh(0,0,-moveModifier);
            }
            break;
            
            
            //----------MAGNET RADIUS
            
        case '=':
        case '+':
            if(adjustMode==ADJUST_MODE_MESH){
                magnetRadius++;
                calculateMagnetPoints();
            }
            break;
            
        case '-':
        case '_':
            if(adjustMode==ADJUST_MODE_MESH){
                magnetRadius--;
                if(magnetRadius<0){
                    magnetRadius=0;
                }
                calculateMagnetPoints();
            }
            break;
            
            //----------SAVE CAMERA AND MESH ADJUSTMENTS
            
        case 's':
            saveCameras();
            break;
            
            
            //----------CREATE NEW MASK/SWITCH MAGNET MODE
            
        case 'm':
            if(adjustMode==ADJUST_MODE_MASK&&bDrawingMask==false){
                cameras[cameraSelect].addMask();
                bNewMask=true;
                bDrawingMask=true;
                updateMasks();
            }
            else if (adjustMode==ADJUST_MODE_MESH){
                magnetMode++;
                if (magnetMode>2){
                    magnetMode=0;
                }
                calculateMagnetPoints();
            }
            break;
            
            //----------DELETE SELECTED MASK
            
        case OF_KEY_BACKSPACE:
            if(adjustMode==ADJUST_MODE_MASK){
                if(cameras[cameraSelect].highlightMask!=-1){
                    cameras[cameraSelect].masks.erase(cameras[cameraSelect].masks.begin()+cameras[cameraSelect].highlightMask);
                    cameras[cameraSelect].drawMasks.erase(cameras[cameraSelect].drawMasks.begin()+cameras[cameraSelect].highlightMask);
                }
                cameras[guiCam].masks=cameras[cameraSelect].masks;
                cameras[guiCam].drawMasks=cameras[cameraSelect].drawMasks;
                updateMasks();
            }
            break;
            
            //----------TOGGLE GUI DRAW
            
        case 'g':
            if(adjustMode!=ADJUST_MODE_LOCKED){
                bDrawGui=!bDrawGui;
            }
            break;
            
            //----------TRIGGER ADJUST_MODE_LOCKED
            
        case ' ':
            if(adjustMode!=ADJUST_MODE_LOCKED){
                bDrawGui=false;
                bDrawWireframe=false;
                adjustMode=ADJUST_MODE_LOCKED;
            }
            else if(bShiftPressed==true){
                adjustMode=ADJUST_MODE_CAMERA;
                bDrawGui=true;
            }
            break;
            
            //----------TOGGLE MESH WIREFRAME DRAW
            
        case 'w':
            if(adjustMode!=ADJUST_MODE_LOCKED){
                bDrawWireframe=!bDrawWireframe;
            }
            break;
            
            
            //----------RELOAD MESH
        
        case 'R':
            //reset mesh to passed through file path to .dae or .obj file
            ofxAssimpModelLoader reload;
            if(meshType==MESH_DETAIL){
                reload.loadModel(detailMesh);
                for(int i=0; i<numMeshes;i++){
                    cameras[cameraSelect].mesh[i]=reload.getMesh(whichMeshes[i]);
                }
            }
            else if(meshType==MESH_MASS){
                reload.loadModel(massMesh);
                for(int i=0; i<numMeshes;i++){
                    cameras[cameraSelect].mesh[i]=reload.getMesh(whichMeshes[i]);
                }
            }
            cout<<"Reloaded Model"<<endl;
            
            break;
    }
}

void ModelMapper::keyReleased(ofKeyEventArgs& args){
    
    //----------TOGGLE SHIFT MODIFIER
    switch(args.key){
        case OF_KEY_SHIFT:
            bShiftPressed=false;
            break;
        case OF_KEY_COMMAND:
            bCommandPressed=false;
            break;
    }
    
    if(bShiftPressed==false&&bCommandPressed==false){
        moveModifier=1;
    }
    
}

void ModelMapper::mouseDragged(ofMouseEventArgs& args){
    
    //----------SETUP SELECT RECTEANGLE
    
    selectRect=ofRectangle(mouseDown,ofVec2f(args.x,args.y));
    
    //----------SELECT MESH ADJUSTMENT POINTS
    
    if(adjustMode==ADJUST_MODE_MESH){
        
        //clear point vector
        moveVertices.clear();
        
        //TODO: add shift-drag to deselect functionality?
        for(int i=0;i<numMeshes;i++)
        {
            vector<meshVertex> tempVector;
            
            //determine vertices inside drag box on camera viewport
            for(int j = 0; j < cameras[cameraSelect].mesh[i].getNumVertices(); j++){
                ofVec3f cur = cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh[i].getVertex(j),cameras[cameraSelect].viewport);
                if(selectRect.inside(cur)) {
                    meshVertex tempVert;
                    tempVert.vertex=cur;
                    tempVert.index=j;
                    tempVector.push_back(tempVert);
                }
            }
            
            //determine vertices inside drag box on gui camera screen
            for(int j = 0; j < cameras[cameraSelect].mesh[i].getNumVertices(); j++){
                ofVec3f cur = cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(j),cameras[guiCam].viewport);
                if(selectRect.inside(cur)) {
                    meshVertex tempVert;
                    tempVert.vertex=cur;
                    tempVert.index=j;
                    tempVector.push_back(tempVert);
                }
            }
            
            //push stored points into vector (to enable shift select command)
            if(tempVertices.size()>0){
                for(int j=0;j < tempVertices[i].size();j++){
                    tempVector.push_back(tempVertices[i][j]);
                }
            }
            
            moveVertices.push_back(tempVector);
            tempVector.clear();
        }
        
    }
    
    //----------SELECT MASK ADJUSTMENT POINTS
    
    else if(adjustMode==ADJUST_MODE_MASK){
        
        //clear selected vertex vector
        maskVertices.clear();
        
        //set up local variables for determining nearest point
        ofVec3f nearestVertex;
        int nearestIndex = 0;
        float drawNearest=false;
        
        //select nearest points
        //TODO: make work on camera screen? will anyone ever use this? either way, make this and mesh vertices the same
        //TODO: add shift select funcitonality. add shift-drag to deselect functionality?
        for(int i=0;i<cameras[cameraSelect].masks.size();i++){
            vector<ofPoint> vertices = cameras[cameraSelect].masks[i].getVertices();
            float nearestDistance = clickThreshold;
            
            //parse through all vertices to determine nearest, if exists
            for(int j = 0; j < vertices.size(); j++) {
                ofVec3f cur = vertices[j];
                float distance = cur.distance(mouse);
                if(selectRect.inside(cur)) {
                    bMaskPoint=true;
                    maskVertex tempVert;
                    tempVert.vertex=cur;
                    tempVert.maskIndex=i;
                    tempVert.vertexIndex=j;
                    maskVertices.push_back(tempVert);
                }
            }
            
        }
        updateMasks();
    }
}

void ModelMapper::mousePressed(ofMouseEventArgs& args){
    
    //determine whether to begin drawing draw box for mouseDragged()
    if(bMouseDown==false){
        selectRect=ofRectangle(0,0,0,0);
        mouseDown=ofVec2f(args.x,args.y);
        bMouseDown=true;
    }
    bMaskPoint=false;
    
    if(adjustMode==ADJUST_MODE_MESH){
        
        //clear selected vertex vector
        if(bShiftPressed==false){
            tempVertices.clear();
        }
        else{
            tempVertices=moveVertices;
        }
        moveVertices.clear();
        
        for(int i=0;i<numMeshes;i++)
        {
            vector<meshVertex> tempVector;
            int n=cameras[cameraSelect].mesh[i].getNumVertices();
            
            //Check for mouse clicks on camera select screen if present highlight this particular vertex
            for(int j = 0; j <n ; j++){
                ofVec3f cur = cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh[i].getVertex(j),cameras[cameraSelect].viewport);
                float distance = cur.distance(ofVec2f(args.x,args.y));
                if(distance < clickThreshold) {
                    meshVertex tempVert;
                    tempVert.vertex=cur;
                    tempVert.index=j;
                    tempVector.push_back(tempVert);
                }
            }
            
            //Check for mouse clicks on gui screen if present highlight this particular vertex
            for(int j = 0; j < n; j++){
                ofVec3f cur = cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(j),cameras[guiCam].viewport);
                float distance = cur.distance(ofVec2f(args.x,args.y));
                if(distance < clickThreshold) {
                    meshVertex tempVert;
                    tempVert.vertex=cur;
                    tempVert.index=j;
                    tempVector.push_back(tempVert);
                }
            }
            
            //shift-click vector of save selected points
            if(tempVertices.size()>0){
                for(int j=0;j < tempVertices[i].size();j++){
                    tempVector.push_back(tempVertices[i][j]);
                }
            }
            
            moveVertices.push_back(tempVector);
            tempVector.clear();
            
            if(magnetMode!=MAGNET_MODE_NONE){
                calculateMagnetPoints();
            }
        }
        
    }
    
    else if (adjustMode==ADJUST_MODE_MASK){
        maskVertices.clear();
        
        //add new mask point
        if(bDrawingMask==true){
            if(ofGetElapsedTimeMillis()-mouseTimer>250){
                mouseTimer=ofGetElapsedTimeMillis();
                if(bNewMask==true){
                    cameras[cameraSelect].masks[cameras[cameraSelect].highlightMask].addVertex(mouse);
                    bNewMask=false;
                }
                else{
                    cameras[cameraSelect].masks[cameras[cameraSelect].highlightMask].lineTo(mouse);
                }
            }
            //double-click to finish mask
            else{
                if(bNewMask==true){
                    bNewMask=false;
                    cameras[cameraSelect].masks.pop_back();
                    bDrawingMask=false;
                }
                else{
                    cameras[cameraSelect].masks[cameras[cameraSelect].highlightMask].close();
                    bNewMask=false;
                    bDrawingMask=false;
                }
            }
        }
        
        else{
            //set up local variables for determining nearest
            ofVec3f nearestVertex;
            int nearestIndex = 0;
            float drawNearest=false;
            
            //select mask points
            for(int i=0;i<cameras[cameraSelect].masks.size();i++){
                vector<ofPoint> vertices = cameras[cameraSelect].masks[i].getVertices();
                float nearestDistance = clickThreshold;
                
                //parse through all vertices to determine nearest, if exists
                for(int j = 0; j < vertices.size(); j++) {
                    ofVec3f cur = vertices[j];
                    float distance = cur.distance(mouse);
                    if(distance < nearestDistance) {
                        bMaskPoint=true;
                        maskVertex tempVert;
                        tempVert.vertex=cur;
                        tempVert.maskIndex=i;
                        tempVert.vertexIndex=j;
                        maskVertices.push_back(tempVert);
                    }
                }
                
                //draw nearest highlight circle on both gui camera and selected camera
                ofNoFill();
                ofSetLineWidth(2);
                if(drawNearest==true){
                    ofSetColor(ofColor::yellow);
                    ofCircle(nearestVertex, 4);
                }
            }
            
            if(drawNearest==false){
                bool bInside=false;
                //select mask
                for(int i=0;i<cameras[cameraSelect].masks.size();i++){
                    ofPolyline poly=cameras[cameraSelect].masks[i];
                    if(poly.inside(mouse)){
                        cameras[cameraSelect].highlightMask=i;
                        bInside=true;
                    }
                }
                if(bInside==false){
                    cameras[cameraSelect].highlightMask=-1;
                }
            }
        }
        updateMasks();
    }
}

//--------------------------------------------------------------
void ModelMapper::mouseReleased(ofMouseEventArgs& args){
    //reset drag box status
    bMouseDown=false;
}


void ModelMapper:: setupCameras() {
    
    //----------SETUP CAMERAS FROM JSON
    
    for(int i=0; i<numCams;i++){
        if (settings["cameras"].size()>i){
            Camera tempCam;
            
            //load Meshes from ply files
            vector<ofMesh>meshes;
            for(int j=0; j<numMeshes;j++){
                ofMesh tempMesh;
                string loader;
                if(meshType==MESH_DETAIL){
                    loader="mesh_detail_"+ofToString(i)+"_"+ofToString(whichMeshes[j])+".ply";
                    
                }
                else if(meshType==MESH_MASS){
                    loader=ofToString("mesh_mass_"+ofToString(i)+"_"+ofToString(whichMeshes[j])+".ply");
                    cout<<loader<<endl;
                }
                tempMesh.load(loader);
                meshes.push_back(tempMesh);
            }
            
            //load Camera data from JSON
            ofVec3f pos = ofVec3f(settings["cameras"][i]["pos"]["x"].asFloat(), settings["cameras"][i]["pos"]["y"].asFloat(), settings["cameras"][i]["pos"]["z"].asFloat());
            ofQuaternion rot = ofQuaternion(settings["cameras"][i]["orientation"]["x"].asFloat(), settings["cameras"][i]["orientation"]["y"].asFloat(), settings["cameras"][i]["orientation"]["z"].asFloat(), settings["cameras"][i]["orientation"]["w"].asFloat());
            ofVec3f viewPos = ofVec3f(settings["cameras"][i]["viewPos"]["x"].asFloat(),settings["cameras"][i]["viewPos"]["y"].asFloat());
            ofVec3f viewSize = ofVec3f(settings["cameras"][i]["viewSize"]["x"].asFloat(),settings["cameras"][i]["viewSize"]["y"].asFloat());
            
            //load mask polyines from JSON vertex points
            vector<ofPolyline> tempMasks;
            for(int j=0;j<settings["cameras"][i]["mask"].size();j++){
                vector<ofPoint> vertices;
                for(int k=0;k<settings["cameras"][i]["mask"][j]["vertices"].size();k++){
                    vertices.push_back(ofPoint(settings["cameras"][i]["mask"][j]["vertices"][k]["x"].asFloat(),settings["cameras"][i]["mask"][j]["vertices"][k]["y"].asFloat()));
                }
                tempMasks.push_back(ofPolyline(vertices));
            }
            
            tempCam.setup(pos, rot, viewPos, viewSize, meshes, tempMasks);
            cameras.push_back(tempCam);
            meshes.clear();
        }
    }
    
    //draw mask ofPaths from Polylines
    updateMasks();
    
}

void ModelMapper:: saveCameras() {
    for(int i=0; i<numCams;i++){
        
        //----------SAVE CAMERA DATA
        
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
        
        //mask vertices
        settings["cameras"][i]["mask"].clear();
        for(int j=0;j<cameras[i].masks.size();j++){
            vector<ofPoint> vertices=cameras[i].masks[j].getVertices();
            for(int k=0;k<vertices.size();k++){
                settings["cameras"][i]["mask"][j]["vertices"][k]["x"]=vertices[k].x;
                settings["cameras"][i]["mask"][j]["vertices"][k]["y"]=vertices[k].y;
            }
        }
        
        //----------SAVE INDIVIDUAL MESH
        
        //save warped mesh objects
        for(int j=0;j<numMeshes;j++){
            string meshname;
            if(meshType==MESH_DETAIL){
                meshname="mesh_detail_"+ofToString(i)+"_"+
                ofToString(whichMeshes[j])+".ply";
            }
            else if(meshType==MESH_MASS){
                meshname="mesh_mass_"+ofToString(i)+"_"+ofToString(whichMeshes[j])+".ply";
            }
            cameras[i].mesh[j].save(meshname);
        }
    }
    
    //----------SAVE JSON SETTINGS DOCUMENT
    
    ofFile file;
    if (!file.open("settings.json", ofFile::WriteOnly)) {
		cout<<"ERROR: ofxJSONElement::save" << "Unable to open " << file.getAbsolutePath() << ".";
	}
    else {
        Json::StyledWriter writer;
        file << writer.write(settings) << endl;
        cout<<"settings.json SAVED";
        file.close();
    }
}

void ModelMapper:: drawGuiText() {
    
    //Check adjustMode and apply relevant text/instructions
    string cameraInfo="'c' to Change Adjust Mode. Current Mode: ";
    string cameraDetails;
    string cameraData;
    int lineHeight=30;
    int lineDraw=cameras[guiCam].viewport.y+lineHeight;
    
    ofFill();
    ofSetColor(0,0,0,127);
    ofRect(cameras[guiCam].viewport.x,cameras[guiCam].viewport.y,500,500);
    
    ofSetColor(255,255,255);
    
    //Draw selected camera number
    ofDrawBitmapString("Adjusting Camera "+ofToString(cameraSelect), cameras[guiCam].viewport.x+10, lineDraw);
    
    if(adjustMode==ADJUST_MODE_CAMERA){
        cameraData+="Camera Position";
        
        ofSetColor(0,255,0);
        ofDrawBitmapString(cameraData,cameras[guiCam].viewport.x+240, lineDraw);
        lineDraw+=lineHeight;
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("(Arrows) Adjust Camera Up/Down/Left Right",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(a/z) Adjust Camera Forwards/Backwards",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        
    }
    else if(adjustMode==ADJUST_MODE_LOOK_AT){
        cameraData+="Camera Orientation";
        
        ofSetColor(0,255,0);
        ofDrawBitmapString(cameraData,cameras[guiCam].viewport.x+240, lineDraw);
        lineDraw+=lineHeight;
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("(Arrows) Adjust Camera Orientation Up/Down/Left Right",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(a/z) Adjust Camera Rotate",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
    }
    else if(adjustMode==ADJUST_MODE_VIEWPORT){
        cameraData+="Camera Viewport";
        
        ofSetColor(0,255,0);
        ofDrawBitmapString(cameraData,cameras[guiCam].viewport.x+240, lineDraw);
        lineDraw+=lineHeight;
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("(Arrows) Adjust Viewport Up/Down/Left Right",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        
    }
    else if(adjustMode==ADJUST_MODE_MESH){
        cameraData+="Mesh Points";
        
        ofSetColor(0,255,0);
        ofDrawBitmapString(cameraData,cameras[guiCam].viewport.x+240, lineDraw);
        lineDraw+=lineHeight;
        
        string magnetModeText;
        
        if(magnetMode==MAGNET_MODE_NONE){
            magnetModeText="None";
        }
        else if(magnetMode==MAGNET_MODE_LINEAR){
            magnetModeText="Linear";
        }
        else if(magnetMode==MAGNET_MODE_EASE){
            magnetModeText="Ease";
        }
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("Magnet Mode: "+magnetModeText,cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("Magnet Radius: "+ofToString(magnetRadius),cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Mouse Click) Select Individual Mesh Point",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Mouse Drag) Select Multiple Mesh Points",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Arrows) Adjust Selected Mesh Point(s) Up/Down/Left Right",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(a/z) Adjust Selected Mesh Point(s) Forwards/Backwards",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(R) Reset Mesh",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
    }
    else if(adjustMode==ADJUST_MODE_MASK){
        cameraData+="Masks";
        
        ofSetColor(0,255,0);
        ofDrawBitmapString(cameraData,cameras[guiCam].viewport.x+240, lineDraw);
        lineDraw+=lineHeight;
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("(m) Create New Mask",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(New Mask Mouse Click) Add Mask Point",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(New Mask Mouse Double Click) Close Mask",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Mouse Click Inside Mask) Select Mask",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Mouse Click on Mask Point) Select Individual Mask Points",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Mouse Drag) Select Mutliple Mask Points",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Arrows) Adjust Selected Mask Point(s) Up/Down/Left Right",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
        ofDrawBitmapString("(Delete) Delete Selected Mask",cameras[guiCam].viewport.x+10,lineDraw);
        lineDraw+=lineHeight;
    }
    
    ofDrawBitmapString("(c) Switch Selected Camera",cameras[guiCam].viewport.x+10,lineDraw);
    lineDraw+=lineHeight;
    ofDrawBitmapString("(s) Save Selected Camera",cameras[guiCam].viewport.x+10,lineDraw);
    lineDraw+=lineHeight;
    ofDrawBitmapString("(Spacebar) Trigger Presentation Mode",cameras[guiCam].viewport.x+10,lineDraw);
    lineDraw+=lineHeight;
    ofDrawBitmapString("(g) Switch GUI view on/off",cameras[guiCam].viewport.x+10,lineDraw);
    lineDraw+=lineHeight;
    ofDrawBitmapString("Framerate: "+ofToString(ofGetFrameRate()), cameras[guiCam].viewport.x+10, lineDraw);
}

void ModelMapper:: drawCameras() {
    for(int i = 0; i < numCams; i++){
        
        if(adjustMode!=ADJUST_MODE_LOCKED||i!=guiCam){
            
            //CREATE AND POPULATE CAMERA
            
            //Begin camera object
            cameras[i].camera.begin(cameras[i].viewport);
            
            for(int j=0;j<numMeshes;j++){
                
                
                textures[0]->bind();
                
                ofSetColor(255,255,255);
                
                cameras[i].mesh[j].draw();
                
                textures[0]->unbind();
                
                //draw mesh wireframe
                if(bDrawWireframe==true){
                    ofSetColor(255,255,255,100);
                    ofSetLineWidth(2);
                    cameras[i].mesh[j].drawWireframe();
                }
            }
            
            //End camera object
            cameras[i].camera.end();
            
        }
        
        //ADJUST_MODE_LOCKED guiCam text
        else{
            ofDrawBitmapString("Presentation Mode Active. Press Shift + Spacebar to unlock and edit", cameras[guiCam].viewport.x+cameras[guiCam].viewport.width/2-300, cameras[guiCam].viewport.y+cameras[guiCam].viewport.height/2);
        }
        
        
    }
}

void ModelMapper::drawHighlights() {
    
    //draw drag and select box, set in mouseDragged()
    if(bMouseDown==true){
        ofSetColor(255,255,255);
        ofNoFill();
        ofSetLineWidth(1);
        ofRect(selectRect);
    }
    
    if(adjustMode==ADJUST_MODE_MESH){
        
        //----------DRAW NEAREST VERTEX

        //determine nearest vertex and highlight yellow, if within clickThreshold distance of vertex
        for(int i=0;i<numMeshes;i++){
            //set up local variables for determining nearest
            ofVec3f nearestVertex;
            int nearestIndex = 0;
            float nearestDistance = clickThreshold;
            float drawNearest=false;
            int n = cameras[cameraSelect].mesh[i].getNumVertices();
            //parse through all vertices to determine nearest, if exists
            for(int j = 0; j < n; j++) {
                ofVec3f cur = cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(j),cameras[guiCam].viewport);
                float distance = cur.distance(mouse);
                if(distance < nearestDistance) {
                    nearestDistance = distance;
                    nearestVertex = cur;
                    nearestIndex=j;
                    drawNearest=true;
                }
            }
            
            //draw nearest highlight circle on both gui camera and selected camera
            ofNoFill();
            ofSetLineWidth(1);
            if(drawNearest==true){
                ofSetColor(ofColor::yellow);
                ofCircle(nearestVertex, 4);
                ofCircle(cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh[i].getVertex(nearestIndex),cameras[cameraSelect].viewport),4);
            }
            
            //----------DRAW SELECTED VERTICES
            
            //draw selected circles on all vertices selected by clicking or via drag box on both gui camera and selected camera, set in mousePressed() and mouseDragged()
            ofSetColor(255,0,0);
            if(moveVertices.size()>i){
                for(int j=0;j<moveVertices[i].size();j++){
                    ofPushMatrix();
                    ofVec3f translate=cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh[i].getVertex(moveVertices[i][j].index),cameras[cameraSelect].viewport);
                    ofTranslate(translate);
                    ofCircle(0,0,4);
                    ofPopMatrix();
                    
                    if(cameraSelect!=guiCam){
                        ofPushMatrix();
                        ofVec3f translate=cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(moveVertices[i][j].index),cameras[guiCam].viewport);
                        ofTranslate(translate);
                        ofCircle(0,0,4);
                        ofPopMatrix();
                    }
                }
            }
            
            if(magnetVertices.size()>i){
                for(int j=0;j<magnetVertices[i].size();j++){
                    
                    ofSetColor(255,ofMap(magnetVertices[i][j].modifier,0,1,255,0),ofMap(magnetVertices[i][j].modifier,0,1,255,0));
                    
                    ofPushMatrix();
                    ofVec3f translate=cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh[i].getVertex(magnetVertices[i][j].index),cameras[cameraSelect].viewport);
                    ofTranslate(translate);
                    ofCircle(0,0,4);
                    ofPopMatrix();
                    
                    if(cameraSelect!=guiCam){
                        ofPushMatrix();
                        ofVec3f translate=cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(magnetVertices[i][j].index),cameras[guiCam].viewport);
                        ofTranslate(translate);
                        ofCircle(0,0,4);
                        ofPopMatrix();
                    }
                }
            }
            
            if(adjustMode==ADJUST_MODE_MESH&&magnetMode!=MAGNET_MODE_NONE){
                ofSetColor(255);
                ofCircle(mouse, magnetRadius);
            }
        }
    }
    
    if(adjustMode==ADJUST_MODE_MASK){
        
        //-----------DRAW NEAREST VERTEX
        
        //determine nearest vertex and highlight yellow, if within clickThreshold distance of vertex
        
        //set up local variables for determining nearest
        ofVec3f nearestVertex;
        int nearestIndex = 0;
        
        for(int i=0;i<cameras[cameraSelect].masks.size();i++){
            
            vector<ofPoint> vertices = cameras[cameraSelect].masks[i].getVertices();
            float nearestDistance = clickThreshold;
            float drawNearest=false;
            
            //parse through all vertices to determine nearest, if exists
            for(int j = 0; j < vertices.size(); j++) {
                ofVec3f cur = vertices[j];
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
            }
        }
        
        ofSetColor(255,0,0);
        for(int i=0;i<maskVertices.size();i++){
            ofCircle(maskVertices[i].vertex,4);
        }
        
    }
    
    //reset line width
	ofSetLineWidth(1);
}

void ModelMapper::drawMasks(){
    
    //draw mask ofPaths
    
    for(int i = 0; i < numCams; i++){
        for(int j=cameras[i].drawMasks.size()-1; j>=0;j--){
            ofSetColor(0,0,0);
            
            //turn on outlines if adjusting
            if(adjustMode==ADJUST_MODE_MASK){
                cameras[i].drawMasks[j].setStrokeColor(ofColor::white);
                if(i==cameraSelect||i==guiCam){
                    if(j==cameras[i].highlightMask&&bMaskPoint==false){
                        cameras[i].drawMasks[j].setStrokeColor(ofColor::yellow);
                    }
                }
            }
            else{
                cameras[i].drawMasks[j].setStrokeColor(ofColor::black);
            }
            cameras[i].drawMasks[j].draw();
        }
    }
}

void ModelMapper::mouseMoved(ofMouseEventArgs& args){
    mouse=ofVec2f(args.x,args.y);
}

void ModelMapper::updateMasks(){
    cameras[guiCam].masks=cameras[cameraSelect].masks;
    cameras[guiCam].drawMasks=cameras[cameraSelect].drawMasks;
    
    //----------CREATE PATH(S)
    //parse through mask polylines and create paths from them. only do when updating so drawing uses less memory
    for(int i=0;i<numCams;i++){
        for(int j=0;j<cameras[i].drawMasks.size();j++){
            vector<ofPoint> vertices=cameras[i].masks[j].getVertices();
            cameras[i].drawMasks[j].clear();
            for (int k=0;k<vertices.size();k++){
                if(k==0){
                    cameras[i].drawMasks[j].moveTo(ofMap(vertices[k].x,cameras[guiCam].viewport.x,cameras[guiCam].viewport.width,cameras[i].viewport.x,cameras[i].viewport.x+cameras[i].viewport.width),ofMap(vertices[k].y,cameras[guiCam].viewport.y,cameras[guiCam].viewport.height,cameras[i].viewport.y,cameras[i].viewport.y+cameras[i].viewport.height));
                    
                }
                else{
                    cameras[i].drawMasks[j].lineTo(ofMap(vertices[k].x,cameras[guiCam].viewport.x,cameras[guiCam].viewport.width,cameras[i].viewport.x,cameras[i].viewport.x+cameras[i].viewport.width),ofMap(vertices[k].y,cameras[guiCam].viewport.y,cameras[guiCam].viewport.height,cameras[i].viewport.y,cameras[i].viewport.y+cameras[i].viewport.height));
                }
            }
            cameras[i].drawMasks[j].close();
            cameras[i].drawMasks[j].setStrokeColor(ofColor::white);
            cameras[i].drawMasks[j].setColor(ofColor::black);
            cameras[i].drawMasks[j].setFilled(true);
            cameras[i].drawMasks[j].setStrokeWidth(1);
        }
    }
    
    
}

void ModelMapper::setMassMesh(string _reloadMesh){
    //pass through reload mesh file path
    massMesh=_reloadMesh;
}

void ModelMapper::setDetailMesh(string _reloadMesh){
    //pass through reload mesh file path
    massMesh=_reloadMesh;
}

void ModelMapper::adjustPosition(float x, float y, float z){
    cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(x,y,z));
}

void ModelMapper::adjustOrientation(float x, float y, float z){
    ofQuaternion yRot(x, ofVec3f(0,1,0));
    ofQuaternion xRot(y, ofVec3f(1,0,0));
    ofQuaternion zRot(z, ofVec3f(0,0,1));
    cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
}

void ModelMapper::adjustMesh(float x, float y, float z){
    if(magnetMode==MAGNET_MODE_NONE){
        for(int i=0;i<moveVertices.size();i++){
            for(int j=0;j<moveVertices[i].size();j++){
                cameras[cameraSelect].mesh[i].setVertex(moveVertices[i][j].index,cameras[cameraSelect].mesh[i].getVertex(moveVertices[i][j].index)+ofVec3f(x,y,z));
            }
        }
    }
    else{
        for(int i=0;i<moveVertices.size();i++){
            for(int j=0;j<moveVertices[i].size();j++){
                cameras[cameraSelect].mesh[i].setVertex(moveVertices[i][j].index,cameras[cameraSelect].mesh[i].getVertex(moveVertices[i][j].index)+ofVec3f(x,y,z));
            }
        }
        for(int i=0;i<magnetVertices.size();i++){
            for(int j=0;j<magnetVertices[i].size();j++){
                cameras[cameraSelect].mesh[i].setVertex(magnetVertices[i][j].index,cameras[cameraSelect].mesh[i].getVertex(magnetVertices[i][j].index)+ofVec3f(x*magnetVertices[i][j].modifier,y*magnetVertices[i][j].modifier,z*magnetVertices[i][j].modifier));
            }
        }
    }
}

void ModelMapper::calculateMagnetPoints(){
    magnetVertices.clear();
    for(int i=0;i<moveVertices.size();i++){
        vector<meshVertex> tempVector;
        for(int j=0;j<moveVertices[i].size();j++){
            int n = cameras[cameraSelect].mesh[i].getNumVertices();
            //parse through all vertices to determine nearest, if exists
            
            int magnetCount;
            
            for(int k = 0; k < n; k++) {
                ofVec3f check = cameras[cameraSelect].mesh[i].getVertex(k);
                ofVec3f cur=cameras[cameraSelect].mesh[i].getVertex(moveVertices[i][j].index);
                float distance = cur.distance(check);
                float maxDist = 0;
                if (distance<magnetRadius&&distance!=0){
                    magnetCount++;
                    meshVertex tempVert;
                    tempVert.vertex=cur;
                    tempVert.index=k;
                    if(magnetMode==MAGNET_MODE_LINEAR){
                        tempVert.modifier=ofMap(distance,0,magnetRadius,1,0);
                    }
                    else if(magnetMode==MAGNET_MODE_EASE) {
                        tempVert.modifier=mapVal(distance,0,magnetRadius,1,0,3);
                    }
                    tempVector.push_back(tempVert);
                }
            }
        }
        magnetVertices.push_back(tempVector);
        tempVector.clear();
    }
}

void ModelMapper::adjustMask(float x, float y){
    
    //Adjust Entire Mask Position
    if(bMaskPoint==false){
        if(cameras[cameraSelect].highlightMask!=-1){
            vector<ofPoint> vertices=cameras[cameraSelect].masks[cameras[cameraSelect].highlightMask].getVertices();
            for(int i=0;i<vertices.size();i++){
                vertices[i]+=ofPoint(x,y);
            }
            cameras[cameraSelect].masks[cameras[cameraSelect].highlightMask]=ofPolyline(vertices);
        }
    }
    
    //Adjust Mask Point Position
    else{
        for(int i=0;i<cameras[cameraSelect].masks.size();i++){
            vector<ofPoint> vertices=cameras[cameraSelect].masks[i].getVertices();
            for (int j=0;j<maskVertices.size();j++){
                if(maskVertices[j].maskIndex==i){
                    for(int k=0;k<vertices.size();k++){
                        if(maskVertices[j].vertexIndex==k){
                            vertices[k]+=ofPoint(x,y);
                            maskVertices[j].vertex+=ofPoint(x,y);
                        }
                    }
                }
            }
            cameras[cameraSelect].masks[i]=ofPolyline(vertices);
        }
    }
    
    //Redraw Mask ofPolylines to ofPaths
    updateMasks();
    
}


float ModelMapper::mapVal(float in, float inMin, float inMax, float outMin, float outMax, float shaper){
    // (1) convert to pct (0-1)
    float pct = ofMap (in, inMin, inMax, 0, 1, true);
    // raise this number to a power
    pct = powf(pct, shaper);
    float out = ofMap(pct, 0,1, outMin, outMax, true);
    return out;
}