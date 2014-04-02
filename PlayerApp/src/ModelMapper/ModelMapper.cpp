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
    selectMode=SELECT_MODE_POINTER;
    easeMode=EASE_MODE_NONE;
    easeInStyle=EASE_STYLE_LINEAR;
    easeOutStyle=EASE_STYLE_LINEAR;
    easeBothStyle=EASE_STYLE_LINEAR;
    
    //mouse settings
    bMouseDown=false;
    clickThreshold=4;
    
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
    
    setupGUI();
    

    
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
    
}

void ModelMapper::keyPressed(ofKeyEventArgs& args){
    
    //set moveModifier to default if no modifier pressed
    if(bShiftPressed==false&&bCommandPressed==false){
        moveModifier=1;
    }
    
    switch(args.key){
            
        //----------MODIFY ADJUSTMENTS
        
        case OF_KEY_COMMAND:
            bCommandPressed=true;
            moveModifier=.1;
            break;
        
        case OF_KEY_SHIFT:
            bShiftPressed=true;
            moveModifier=10;
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
                adjustViewport(0,-moveModifier);
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
                adjustViewport(0,moveModifier);
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
                adjustViewport(moveModifier,0);
                
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
                adjustViewport(-moveModifier,0);
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
        
            //----------CREATE NEW MASK/SWITCH MAGNET MODE
            
        case 'm':
            if(adjustMode==ADJUST_MODE_MASK&&bDrawingMask==false){
                cameras[cameraSelect].addMask();
                bNewMask=true;
                bDrawingMask=true;
                updateMasks();
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
                if(mainGUI->isVisible()){
                    mainGUI->setVisible(false);
                    positionGUI->setVisible(false);
                    orientationGUI->setVisible(false);
                    viewportGUI->setVisible(false);
                    meshGUI->setVisible(false);
                    magnetGUI->setVisible(false);
                }
                else{
                    mainGUI->setVisible(true);
                    if(adjustMode==ADJUST_MODE_CAMERA) positionGUI->setVisible(true);
                    else if(adjustMode==ADJUST_MODE_LOOK_AT) orientationGUI->setVisible(true);
                    else if(adjustMode==ADJUST_MODE_VIEWPORT) viewportGUI->setVisible(true);
                    else if(adjustMode==ADJUST_MODE_MESH) {
                        meshGUI->setVisible(true);
                        if(selectMode!=SELECT_MODE_POINTER){
                            magnetGUI->setVisible(true);
                        }
                    }
                }
            }
            else if(bShiftPressed==true){
                mainGUI->setVisible(true);
                positionGUI->setVisible(true);
            }
            break;
            
            //----------TRIGGER ADJUST_MODE_LOCKED
            
        case ' ':
            if(adjustMode!=ADJUST_MODE_LOCKED){
                bDrawGui=false;
                bDrawWireframe=false;
                wireframeButton->setColorBack(OFX_UI_COLOR_BACK);
                adjustMode=ADJUST_MODE_LOCKED;
                mainGUI->setVisible(false);
                positionGUI->setVisible(false);
                orientationGUI->setVisible(false);
                viewportGUI->setVisible(false);
                meshGUI->setVisible(false);
                magnetGUI->setVisible(false);
            }
            else if(bShiftPressed==true){
                adjustMode=ADJUST_MODE_CAMERA;
                bDrawGui=true;
                mainGUI->setVisible(true);
                
                
                if(adjustMode==ADJUST_MODE_CAMERA) positionGUI->setVisible(true);
                else if(adjustMode==ADJUST_MODE_LOOK_AT) orientationGUI->setVisible(true);
                else if(adjustMode==ADJUST_MODE_VIEWPORT) viewportGUI->setVisible(true);
                else if(adjustMode==ADJUST_MODE_MESH) {
                    meshGUI->setVisible(true);
                    if(selectMode!=SELECT_MODE_POINTER){
                        magnetGUI->setVisible(true);
                    }
                }
                currentMode->activateToggle("Camera Position");
                performanceButton->setColorBack(OFX_UI_COLOR_BACK);
            }
            break;
            
            //----------TOGGLE MESH WIREFRAME DRAW
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
            
            //determine vertices inside drag box on gui camera screen
            for(int j = 0; j < cameras[guiCam].mesh[i].getNumVertices(); j++){
                ofVec3f cur = cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(j),cameras[guiCam].viewport);
                if(selectRect.inside(cur)) {
                    meshVertex tempVert;
                    tempVert.vertex=cur;
                    tempVert.index=j;
                    bool included=false;
                    if(tempVertices.size()>0){
                        for(int k=0;k<tempVertices[i].size();k++){
                            if(tempVertices[i][k].index==tempVert.index){
                                included=true;
                                if(bShiftPressed==true){
                                    tempVertices[i].erase(tempVertices[i].begin()+k);
                                }
                            }
                        }
                        if(bShiftPressed==false&&included==false){
                            tempVector.push_back(tempVert);
                        }
                    }
                    else if(bShiftPressed==false){
                        tempVector.push_back(tempVert);
                    }
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
        if(selectMode!=SELECT_MODE_POINTER){
            calculateMagnetPoints();
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
            tempVertices=moveVertices;
            moveVertices.clear();
            
            for(int i=0;i<numMeshes;i++)
            {
                vector<meshVertex> tempVector;
                int n=cameras[guiCam].mesh[i].getNumVertices();

                //Check for mouse clicks on gui screen if present highlight this particular vertex
                for(int j = 0; j < n; j++){
                    ofVec3f cur = cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(j),cameras[guiCam].viewport);
                    float distance = cur.distance(ofVec2f(args.x,args.y));
                    if(distance < clickThreshold) {
                        meshVertex tempVert;
                        tempVert.vertex=cur;
                        tempVert.index=j;
                        bool included=false;
                        if(tempVertices.size()>0){
                            for(int k=0;k<tempVertices[i].size();k++){
                                if(tempVertices[i][k].index==tempVert.index){
                                    included=true;
                                    if(bShiftPressed==true){
                                        tempVertices[i].erase(tempVertices[i].begin()+k);
                                    }
                                }
                            }
                            if(bShiftPressed==false&&included==false){
                                tempVector.push_back(tempVert);
                            }
                        }
                        else if(bShiftPressed==false){
                            tempVector.push_back(tempVert);
                        }
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
                
                if(selectMode!=SELECT_MODE_POINTER){
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

void ModelMapper:: saveCamera() {
        
        //----------SAVE CAMERA DATA
        
        //Camera position
        settings["cameras"][cameraSelect]["pos"]["x"]=cameras[cameraSelect].camera.getGlobalPosition().x;
        settings["cameras"][cameraSelect]["pos"]["y"]=cameras[cameraSelect].camera.getGlobalPosition().y;
        settings["cameras"][cameraSelect]["pos"]["z"]=cameras[cameraSelect].camera.getGlobalPosition().z;
        
        //camera viewport offset
        settings["cameras"][cameraSelect]["viewPos"]["x"]=cameras[cameraSelect].viewport.x;
        settings["cameras"][cameraSelect]["viewPos"]["y"]=cameras[cameraSelect].viewport.y;
        
        //camera orientation quaternion
        settings["cameras"][cameraSelect]["orientation"]["x"]=cameras[cameraSelect].camera.getGlobalOrientation().x();
        settings["cameras"][cameraSelect]["orientation"]["y"]=cameras[cameraSelect].camera.getGlobalOrientation().y();
        settings["cameras"][cameraSelect]["orientation"]["z"]=cameras[cameraSelect].camera.getGlobalOrientation().z();
        settings["cameras"][cameraSelect]["orientation"]["w"]=cameras[cameraSelect].camera.getGlobalOrientation().w();
        
        //mask vertices
        settings["cameras"][cameraSelect]["mask"].clear();
        for(int j=0;j<cameras[cameraSelect].masks.size();j++){
            vector<ofPoint> vertices=cameras[cameraSelect].masks[j].getVertices();
            for(int k=0;k<vertices.size();k++){
                settings["cameras"][cameraSelect]["mask"][j]["vertices"][k]["x"]=vertices[k].x;
                settings["cameras"][cameraSelect]["mask"][j]["vertices"][k]["y"]=vertices[k].y;
            }
        }
        
        //----------SAVE INDIVIDUAL MESH
        
        //save warped mesh objects
        for(int j=0;j<numMeshes;j++){
            string meshname;
            if(meshType==MESH_DETAIL){
                meshname="mesh_detail_"+ofToString(cameraSelect)+"_"+
                ofToString(whichMeshes[j])+".ply";
            }
            else if(meshType==MESH_MASS){
                meshname="mesh_mass_"+ofToString(cameraSelect)+"_"+ofToString(whichMeshes[j])+".ply";
            }
            cameras[cameraSelect].mesh[j].save(meshname);
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
                    
                    if(translate.x>cameras[cameraSelect].viewport.x&&translate.x<cameras[cameraSelect].viewport.x+cameras[cameraSelect].viewport.width){
                        ofTranslate(translate);
                        ofCircle(0,0,4);
                    }
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
            
            if(adjustMode==ADJUST_MODE_MESH&&selectMode!=SELECT_MODE_POINTER){
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
    
    if(positionX!=NULL){
        positionX->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalPosition().x));
    }
    if(positionY!=NULL){
        positionY->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalPosition().y));
    }
    if(positionZ!=NULL){
        positionZ->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalPosition().z));
    }
    
}

void ModelMapper::adjustOrientation(float x, float y, float z){
    ofQuaternion yRot(x, ofVec3f(0,1,0));
    ofQuaternion xRot(y, ofVec3f(1,0,0));
    ofQuaternion zRot(z, ofVec3f(0,0,1));
    cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
    
    if(orientationX!=NULL){
        orientationX->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().x()));
    }
    if(orientationY!=NULL){
        orientationY->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().y()));
    }
    if(orientationZ!=NULL){
        orientationZ->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().z()));
    }
    if(orientationW!=NULL){
        orientationW->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().w()));
    }
}

void ModelMapper::adjustViewport(float x, float y){
    cameras[cameraSelect].viewport.x+=x;
    cameras[cameraSelect].viewport.y+=y;
    if(viewportX!=NULL){
        viewportX->setTextString(ofToString(cameras[cameraSelect].viewport.x));
    }
    if(viewportY!=NULL){
        viewportY->setTextString(ofToString(cameras[cameraSelect].viewport.y));
    }
    
}

void ModelMapper::adjustMesh(float x, float y, float z){
    if(selectMode==SELECT_MODE_POINTER){
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
            int n = cameras[guiCam].mesh[i].getNumVertices();
            //parse through all vertices to determine nearest, if exists
            
            int magnetCount;
            
            for(int k = 0; k < n; k++) {
                ofVec3f check = cameras[guiCam].mesh[i].getVertex(k);
                ofVec3f cur=cameras[guiCam].mesh[i].getVertex(moveVertices[i][j].index);
                float distance = cur.distance(check);
                float maxDist = 0;
                if (distance<magnetRadius&&distance!=0){
                    magnetCount++;
                    meshVertex tempVert;
                    bool duplicate=false;
                    bool selected=false;
                    for(int l=0;l<tempVector.size();l++){
                        if(tempVector[l].index==k){
                            cout<<"recheck: "<<k<<endl;
                            duplicate=true;
                            if(distance>check.distance(tempVector[l].vertex)){
                                tempVert.vertex=cur;
                                if(selectMode==SELECT_MODE_RADIUS||selectMode==SELECT_MODE_PEN){
                                    tempVert.modifier=magnetMap(distance, magnetRadius);
                                }
                            }
                            break;
                        }
                    }
                    
                    for(int l=0;l<moveVertices[i].size();l++){
                        if(moveVertices[i][l].index==k){
                            selected=true;
                        }
                    }
                    
                    if(duplicate==false&&selected==false){
                        tempVert.vertex=cur;
                        tempVert.index=k;
                        if(selectMode==SELECT_MODE_RADIUS||selectMode==SELECT_MODE_PEN){
                            tempVert.modifier=magnetMap(distance, magnetRadius);
                        }
                        tempVector.push_back(tempVert);
                    }
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

void ModelMapper::setMainGUI(){
    mainGUI = new ofxUISuperCanvas("Model Mapper");
    mainGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    mainGUI->addSpacer();
    mainGUI->addLabel("Press 'g' to Hide GUIs", OFX_UI_FONT_SMALL);
    
    mainGUI->addSpacer();
    mainGUI->addLabel("Current Camera",OFX_UI_FONT_MEDIUM);
	mainGUI->addRadio("CURRENT", cameraNames, OFX_UI_ORIENTATION_HORIZONTAL, OFX_UI_FONT_MEDIUM)->activateToggle("1");
    
    
    mainGUI->addSpacer();
    mainGUI->addLabel("Adjust:",OFX_UI_FONT_MEDIUM);
	currentMode=mainGUI->addRadio("CHANGE MODE", adjustModes, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    currentMode->activateToggle("Camera Position");
    
    mainGUI->addSpacer();
    wireframeButton=mainGUI->addLabelButton("TOGGLE WIREFRAME", false);
    
    mainGUI->addSpacer();
    mainGUI->addLabelButton("SAVE CURRENT", false);
    mainGUI->addLabelButton("SAVE ALL", false);
    
    mainGUI->addSpacer();
    performanceButton=mainGUI->addLabelButton("PERFORMANCE MODE", false);
    
    mainGUI->autoSizeToFitWidgets();
	ofAddListener(mainGUI->newGUIEvent,this,&ModelMapper::guiEvent);
}

void ModelMapper::setPositionGUI(){
	
	positionGUI = new ofxUISuperCanvas("Camera Position");
    
    positionGUI->addSpacer();
    positionGUI->addLabel("(Arrow Keys) Adjust Camera x/y", OFX_UI_FONT_SMALL);
    positionGUI->addLabel("(z/a) Adjust Camera z", OFX_UI_FONT_SMALL);
    positionGUI->addLabel("(Shift) for fast move", OFX_UI_FONT_SMALL);
    positionGUI->addLabel("(Cmd) for slow move", OFX_UI_FONT_SMALL);
    
    positionGUI->addSpacer();
    positionGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    positionGUI->addLabel("Camera X",OFX_UI_FONT_MEDIUM);
    positionX = positionGUI->addTextInput("Camera X", ofToString(cameras[cameraSelect].camera.getGlobalPosition().x));
    positionX->setAutoClear(false);
    positionGUI->addLabel("Camera Y",OFX_UI_FONT_MEDIUM);
    positionY = positionGUI->addTextInput("Camera Y", ofToString(cameras[cameraSelect].camera.getGlobalPosition().y));
    positionY->setAutoClear(false);
    positionGUI->addLabel("Camera Z",OFX_UI_FONT_MEDIUM);
    positionZ = positionGUI->addTextInput("Camera Z", ofToString(cameras[cameraSelect].camera.getGlobalPosition().z));
    positionZ->setAutoClear(false);
    
    positionGUI->setPosition(212, 0);
    
    positionGUI->autoSizeToFitWidgets();
	ofAddListener(positionGUI->newGUIEvent,this,&ModelMapper::guiEvent);
}

void ModelMapper::setOrientationGUI(){
    orientationGUI = new ofxUISuperCanvas("Camera Orientation");
    
    orientationGUI->addSpacer();
    orientationGUI->addLabel("(Arrow Keys) Adjust Orientation x/y", OFX_UI_FONT_SMALL);
    orientationGUI->addLabel("(z/a) Adjust Orientation z", OFX_UI_FONT_SMALL);
    orientationGUI->addLabel("(Shift) for fast move", OFX_UI_FONT_SMALL);
    orientationGUI->addLabel("(Cmd) for slow move", OFX_UI_FONT_SMALL);
    
    orientationGUI->addSpacer();
    orientationGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    
    orientationGUI->addLabel("Orientation X",OFX_UI_FONT_MEDIUM);
    orientationX = orientationGUI->addTextInput("Orientation X", ofToString(cameras[cameraSelect].camera.getGlobalOrientation().x()));
    orientationX->setAutoClear(false);
    
    orientationGUI->addLabel("Orientation Y",OFX_UI_FONT_MEDIUM);
    orientationY = orientationGUI->addTextInput("Orientation Y", ofToString(cameras[cameraSelect].camera.getGlobalOrientation().y()));
    orientationY->setAutoClear(false);
    
    orientationGUI->addLabel("Orientation Z",OFX_UI_FONT_MEDIUM);
    orientationZ = orientationGUI->addTextInput("Orientation Z", ofToString(cameras[cameraSelect].camera.getGlobalOrientation().z()));
    orientationZ->setAutoClear(false);
    
    orientationGUI->addLabel("Orientation W",OFX_UI_FONT_MEDIUM);
    orientationW = orientationGUI->addTextInput("Orientation W", ofToString(cameras[cameraSelect].camera.getGlobalOrientation().w()));
    orientationW->setAutoClear(false);
    
    orientationGUI->setPosition(212, 0);
    orientationGUI->autoSizeToFitWidgets();
    
    orientationGUI->setVisible(false);
	ofAddListener(orientationGUI->newGUIEvent,this,&ModelMapper::guiEvent);
}

void ModelMapper::setViewportGUI(){
    viewportGUI = new ofxUISuperCanvas("Viewport Position");
    
    viewportGUI->addSpacer();
    viewportGUI->addLabel("(Arrow Keys) Adjust Viewport x/y", OFX_UI_FONT_SMALL);
    viewportGUI->addLabel("(Shift) for fast move", OFX_UI_FONT_SMALL);
    viewportGUI->addLabel("(Cmd) for slow move", OFX_UI_FONT_SMALL);
    
    viewportGUI->addSpacer();
    viewportGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    
    viewportGUI->addLabel("Viewport X",OFX_UI_FONT_MEDIUM);
    viewportX = viewportGUI->addTextInput("Viewport X", ofToString(cameras[cameraSelect].viewport.x));
    viewportX->setAutoClear(false);
    
    viewportGUI->addLabel("Viewport Y",OFX_UI_FONT_MEDIUM);
    viewportY = viewportGUI->addTextInput("Viewport Y", ofToString(cameras[cameraSelect].viewport.y));
    viewportY->setAutoClear(false);
    
    viewportGUI->setPosition(212, 0);
    viewportGUI->autoSizeToFitWidgets();
    
    viewportGUI->setVisible(false);
	ofAddListener(viewportGUI->newGUIEvent,this,&ModelMapper::guiEvent);
}

void ModelMapper::setMeshGUI(){
    meshGUI = new ofxUISuperCanvas("Mesh");
    meshGUI->addSpacer();
    meshGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    meshGUI->addLabel("Click vertices to select", OFX_UI_FONT_SMALL);
    meshGUI->addLabel("Drag over to select multiple", OFX_UI_FONT_SMALL);
    meshGUI->addLabel("Shift-click to remove multiple", OFX_UI_FONT_SMALL);
    meshGUI->addLabel("(Arrow Keys) Adjust x/y", OFX_UI_FONT_SMALL);
    meshGUI->addLabel("(z/a) Adjust z", OFX_UI_FONT_SMALL);
    meshGUI->addLabel("(Shift) for fast move", OFX_UI_FONT_SMALL);
    meshGUI->addLabel("(Cmd) for slow move", OFX_UI_FONT_SMALL);
    
    vector<string> magnetMethods;
    magnetMethods.push_back("Pointer");
    magnetMethods.push_back("Radius");
    magnetMethods.push_back("Pen");
    magnetMethods.push_back("Double Pen");
    
    meshGUI->addSpacer();
    meshGUI->addLabel("Selection:",OFX_UI_FONT_MEDIUM);
	meshGUI->addRadio("SELECTION TYPE", magnetMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM)->activateToggle("Pointer");
    
    meshGUI->addSpacer();
    meshGUI->addLabelButton("CLEAR SELECTION", false);
    
    meshGUI->addSpacer();
    meshGUI->addLabelButton("RELOAD MESH", false);
    
    meshGUI->setPosition(212, 0);
    meshGUI->autoSizeToFitWidgets();
    
    meshGUI->setVisible(false);
	ofAddListener(meshGUI->newGUIEvent,this,&ModelMapper::guiEvent);
    
}

void ModelMapper::setMagnetGUI(){
    magnetGUI = new ofxUISuperCanvas("Selection");
    
    vector<string> easeTypes;
    easeTypes.push_back("None");
    easeTypes.push_back("Ease In");
    easeTypes.push_back("Ease Out");
    easeTypes.push_back("Ease Both");
    easeTypes.push_back("Ease Separate");
    
    magnetGUI->addSpacer();
    magnetGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    magnetRadiusLabel=magnetGUI->addLabel("Selection Radius",OFX_UI_FONT_MEDIUM);
    magnetRadiusSet = magnetGUI->addTextInput("Selection Radius", ofToString(magnetRadius));
    magnetRadiusSet->setAutoClear(false);
    
    penButton=magnetGUI->addLabelButton("START SELECTION", OFX_UI_FONT_MEDIUM);
    doublePenButton=magnetGUI->addLabelButton("START RADIUS", OFX_UI_FONT_MEDIUM);
    
    magnetRadiusSpacer=magnetGUI->addSpacer();
    easeTypeLabel=magnetGUI->addLabel("Ease Type:");
    easeTypeRadio=magnetGUI->addRadio("EASE TYPE", easeTypes, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    easeTypeRadio->activateToggle("None");
    
    easeTypeSpacer=magnetGUI->addSpacer();
    easeInLabel = magnetGUI->addLabel("Ease In Method:");
    easeInMethod = magnetGUI->addRadio("EASE IN", easeMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    
    easeOutLabel = magnetGUI->addLabel("Ease Out Method:");
    easeOutMethod = magnetGUI->addRadio("EASE OUT", easeMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    
    easeBothLabel = magnetGUI->addLabel("Ease Both Method:");
    easeBothMethod = magnetGUI->addRadio("EASE BOTH", easeMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    
    magnetGUI->setPosition(212*2, 0);
    magnetGUI->setHeight(250);
    
    magnetGUI->setVisible(false);
	ofAddListener(magnetGUI->newGUIEvent,this,&ModelMapper::guiEvent);
}

void ModelMapper::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	int kind = e.getKind();
    
	cout << "got event from: " << name << endl;
    cout<<"kind: "<<kind<<endl;
    
    
    if(name=="TOGGLE WIREFRAME"){
        if(adjustMode!=ADJUST_MODE_LOCKED){
            bDrawWireframe=!bDrawWireframe;
            if(bDrawWireframe==true){
                wireframeButton->setColorBack(OFX_UI_COLOR_FILL);
            }
            else{
                wireframeButton->setColorBack(OFX_UI_COLOR_BACK);
            }
        }
    }
    
    else if(name=="PERFORMANCE MODE"){
        bDrawGui=false;
        bDrawWireframe=false;
        wireframeButton->setColorBack(OFX_UI_COLOR_BACK);
        performanceButton->setColorBack(OFX_UI_COLOR_BACK);
        adjustMode=ADJUST_MODE_LOCKED;
        mainGUI->setVisible(false);
        positionGUI->setVisible(false);
        orientationGUI->setVisible(false);
        viewportGUI->setVisible(false);
        meshGUI->setVisible(false);
        magnetGUI->setVisible(false);
    }
    
    else if(name == "Camera X")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].camera.setGlobalPosition(ofVec3f(ofToFloat(ti->getTextString()),cameras[cameraSelect].camera.getGlobalPosition().y,cameras[cameraSelect].camera.getGlobalPosition().z));
            cout<<ofToString(cameras[cameraSelect].camera.getGlobalPosition().x)<<endl;
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Camera Y")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].camera.setGlobalPosition(ofVec3f(cameras[cameraSelect].camera.getGlobalPosition().x,ofToFloat(ti->getTextString()),cameras[cameraSelect].camera.getGlobalPosition().z));
            cout<<ofToString(cameras[cameraSelect].camera.getGlobalPosition().y)<<endl;
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Camera Z")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].camera.setGlobalPosition(ofVec3f(cameras[cameraSelect].camera.getGlobalPosition().x,cameras[cameraSelect].camera.getGlobalPosition().y,ofToFloat(ti->getTextString())));
            cout<<ofToString(cameras[cameraSelect].camera.getGlobalPosition().z)<<endl;
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Orientation X")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(ofToFloat(ti->getTextString()), cameras[cameraSelect].camera.getGlobalOrientation().y(), cameras[cameraSelect].camera.getGlobalOrientation().z(), cameras[cameraSelect].camera.getGlobalOrientation().w());
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);

            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Orientation Y")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(cameras[cameraSelect].camera.getGlobalOrientation().x(),ofToFloat(ti->getTextString()), cameras[cameraSelect].camera.getGlobalOrientation().z(), cameras[cameraSelect].camera.getGlobalOrientation().w());
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);
            
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Orientation Z")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(cameras[cameraSelect].camera.getGlobalOrientation().x(), cameras[cameraSelect].camera.getGlobalOrientation().y(),ofToFloat(ti->getTextString()), cameras[cameraSelect].camera.getGlobalOrientation().w());
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);
            
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Orientation W")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(cameras[cameraSelect].camera.getGlobalOrientation().x(), cameras[cameraSelect].camera.getGlobalOrientation().y(), cameras[cameraSelect].camera.getGlobalOrientation().z(),ofToFloat(ti->getTextString()));
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);
            
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Viewport X")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].viewport.x=ofToFloat(ti->getTextString());
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "Viewport Y")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].viewport.y=ofToFloat(ti->getTextString());
            cout<<"set"<<endl;
        }
    }
    
    else if(name == "SAVE ALL")
	{
		ofxUIButton *button = (ofxUIButton *) e.getButton();
		if(button->getValue()==1){
            saveCameras();
        }
	}
    
    else if(name == "SAVE CURRENT")
	{
		ofxUIButton *button = (ofxUIButton *) e.getButton();
		if(button->getValue()==1){
            saveCamera();
        }
	}
    
    else if(name == "CURRENT")
    {
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        cameraSelect=ofToInt(currentRadio->getActiveName());
        updateMasks();
        if(positionX!=NULL){
            positionX->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalPosition().x));
        }
        if(positionY!=NULL){
            positionY->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalPosition().y));
        }
        if(positionZ!=NULL){
            positionZ->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalPosition().z));
        }
        if(orientationX!=NULL){
            orientationX->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().x()));
        }
        if(orientationY!=NULL){
            orientationY->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().y()));
        }
        if(orientationZ!=NULL){
            orientationZ->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().z()));
        }
        if(orientationW!=NULL){
            orientationW->setTextString(ofToString(cameras[cameraSelect].camera.getGlobalOrientation().w()));
        }
        if(viewportX!=NULL){
            viewportX->setTextString(ofToString(cameras[cameraSelect].viewport.x));
        }
        if(viewportY!=NULL){
            viewportY->setTextString(ofToString(cameras[cameraSelect].viewport.y));
        }
    }
    
    else if(name == "CHANGE MODE"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        
            positionGUI->setVisible(false);
            orientationGUI->setVisible(false);
            viewportGUI->setVisible(false);
            meshGUI->setVisible(false);
            magnetGUI->setVisible(false);
        
                if(selected=="Camera Position"){
                    positionGUI->setVisible(true);
                    adjustMode=ADJUST_MODE_CAMERA;
                }
        
                else if(selected=="Camera Orientation"){
                    orientationGUI->setVisible(true);
                    adjustMode=ADJUST_MODE_LOOK_AT;
                }
        
                else if(selected=="Viewport Position"){
                    viewportGUI->setVisible(true);
                    adjustMode=ADJUST_MODE_VIEWPORT;
                }
        
                else if(selected=="Meshes"){
                    meshGUI->setVisible(true);
                    adjustMode=ADJUST_MODE_MESH;
                    if(selectMode!=SELECT_MODE_POINTER){
                        magnetGUI->setVisible(true);
                    }
                }
    }
    
    else if(name=="SELECTION TYPE"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        if(selected=="Pointer"){
            magnetGUI->setVisible(false);
            hideMagnetTypes();
            selectMode=SELECT_MODE_POINTER;
            setEaseHeights(false,false,false);
            calculateMagnetPoints();
        }
        else{
            magnetGUI->setVisible(true);
            easeTypeRadio->activateToggle("None");
            hideMagnetTypes();
            if(selected=="Radius"){
                selectMode=SELECT_MODE_RADIUS;
                magnetRadiusLabel->setVisible(true);
                magnetRadiusSet->setVisible(true);
                setEaseHeights(true,false,false);
                calculateMagnetPoints();
            }
            else if(selected=="Pen"){
                selectMode=SELECT_MODE_PEN;
                magnetRadiusLabel->setVisible(true);
                magnetRadiusSet->setVisible(true);
                setEaseHeights(true,true,false);
                calculateMagnetPoints();
            }
            else if(selected=="Double Pen"){
                selectMode=SELECT_MODE_DOUBLE_PEN;
                setEaseHeights(false, true, true);
                calculateMagnetPoints();
            }
        }
    }
    
    else if(name=="Selection Radius"){
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            magnetRadius=ofToFloat(ti->getTextString());
            calculateMagnetPoints();
        }
    }
    
    else if(name=="CLEAR SELECTION"){
        moveVertices.clear();
        tempVertices.clear();
        magnetVertices.clear();
    }
    
    else if(name=="RELOAD MESH"){
//        moveVertices.clear();
//        tempVertices.clear();
//        magnetVertices.clear();
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
    }
    
    else if(name=="EASE TYPE"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
                if(selected=="None"){
                    hideMagnetTypes();
                    easeMode=EASE_MODE_NONE;
                    calculateMagnetPoints();
                }
                else if(selected=="Ease In"){
                    hideMagnetTypes();
                    easeInLabel->setVisible(true);
                    easeInMethod->setVisible(true);
                    easeInLabel->getRect()->setY(easeHeight);
                    easeInMethod->getRect()->setY(easeHeight+20);
                    magnetGUI->setHeight(400);
                    easeMode=EASE_MODE_IN;
                    calculateMagnetPoints();
                }
                else if(selected=="Ease Out"){
                    hideMagnetTypes();
                    easeOutLabel->setVisible(true);
                    easeOutMethod->setVisible(true);
                    easeOutLabel->getRect()->setY(easeHeight);
                    easeOutMethod->getRect()->setY(easeHeight+20);
                    magnetGUI->setHeight(400);
                    easeMode=EASE_MODE_OUT;
                    calculateMagnetPoints();
                }
                else if(selected=="Ease Both"){
                    hideMagnetTypes();
                    easeBothLabel->setVisible(true);
                    easeBothMethod->setVisible(true);
                    easeBothLabel->getRect()->setY(easeHeight);
                    easeBothMethod->getRect()->setY(easeHeight+20);
                    magnetGUI->setHeight(400);
                    easeMode=EASE_MODE_BOTH;
                    calculateMagnetPoints();
                }
                else if(selected=="Ease Separate"){
                    hideMagnetTypes();
                    easeInLabel->setVisible(true);
                    easeInMethod->setVisible(true);
                    easeOutLabel->setVisible(true);
                    easeOutMethod->setVisible(true);
                    easeInLabel->getRect()->setY(easeHeight);
                    easeInMethod->getRect()->setY(easeHeight+20);
                    easeOutLabel->getRect()->setY(easeHeight+170);
                    easeOutMethod->getRect()->setY(easeHeight+190);
                    magnetGUI->setHeight(580);
                    easeMode==EASE_MODE_SEPARATE;
                    calculateMagnetPoints();
                }
    }
    
    else if(name=="EASE IN"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        if(selected=="Linear"){
            easeInStyle=EASE_STYLE_LINEAR;
        }
        else if(selected=="Quadratic"){
            easeInStyle=EASE_STYLE_QUAD;
        }
        else if(selected=="Cubic"){
            easeInStyle=EASE_STYLE_CUBIC;
        }
        else if(selected=="Quartic"){
            easeInStyle=EASE_STYLE_QUART;
        }
        else if(selected=="Exponential"){
            easeInStyle=EASE_STYLE_EXPO;
        }
        else if(selected=="Sine"){
            easeInStyle=EASE_STYLE_SINE;
        }
        else if(selected=="Circular"){
            easeInStyle=EASE_STYLE_CIRC;
        }
        calculateMagnetPoints();
    }
    else if(name=="EASE OUT"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        if(selected=="Linear"){
            easeOutStyle=EASE_STYLE_LINEAR;
        }
        else if(selected=="Quadratic"){
            easeOutStyle=EASE_STYLE_QUAD;
        }
        else if(selected=="Cubic"){
            easeOutStyle=EASE_STYLE_CUBIC;
        }
        else if(selected=="Quartic"){
            easeOutStyle=EASE_STYLE_QUART;
        }
        else if(selected=="Exponential"){
            easeOutStyle=EASE_STYLE_EXPO;
        }
        else if(selected=="Sine"){
            easeOutStyle=EASE_STYLE_SINE;
        }
        else if(selected=="Circular"){
            easeOutStyle=EASE_STYLE_CIRC;
        }
        calculateMagnetPoints();
    }
    else if(name=="EASE BOTH"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        if(selected=="Linear"){
            easeBothStyle=EASE_STYLE_LINEAR;
        }
        else if(selected=="Quadratic"){
            easeBothStyle=EASE_STYLE_QUAD;
        }
        else if(selected=="Cubic"){
            easeBothStyle=EASE_STYLE_CUBIC;
        }
        else if(selected=="Quartic"){
            easeBothStyle=EASE_STYLE_QUART;
        }
        else if(selected=="Exponential"){
            easeBothStyle=EASE_STYLE_EXPO;
        }
        else if(selected=="Sine"){
            easeBothStyle=EASE_STYLE_SINE;
        }
        else if(selected=="Circular"){
            easeBothStyle=EASE_STYLE_CIRC;
        }
        calculateMagnetPoints();
    }
}

void ModelMapper::hideMagnetTypes(){
    magnetGUI->setHeight(250);
    easeInLabel->setVisible(false);
    easeInMethod->setVisible(false);
    easeInMethod->activateToggle("Linear");
    easeOutLabel->setVisible(false);
    easeOutMethod->setVisible(false);
    easeOutMethod->activateToggle("Linear");
    easeBothLabel->setVisible(false);
    easeBothMethod->setVisible(false);
    easeBothMethod->activateToggle("Linear");
}

void ModelMapper::setEaseHeights(bool radiusSpacer, bool pen1, bool pen2){
    
    magnetRadiusLabel->setVisible(radiusSpacer);
    magnetRadiusSet->setVisible(radiusSpacer);
    penButton->setVisible(pen1);
    doublePenButton->setVisible(pen2);
    int start;
    
    if(radiusSpacer){
        start=80;
    }
    else{
        start=30;
    }
    
    if(pen2){
        doublePenButton->getRect()->setY(start+30);
        penButton->getRect()->setY(start);
        easeTypeHeight=start+70;
        magnetRadiusSpacer->getRect()->setY(start+66);
    }
    else if(pen1){
        easeTypeHeight=start+30;
        penButton->getRect()->setY(start);
        magnetRadiusSpacer->getRect()->setY(start+26);
    }
    else{
        easeTypeHeight=start;
        magnetRadiusSpacer->getRect()->setY(start-8);
    }
    easeHeight=137+easeTypeHeight;
    easeTypeLabel->getRect()->setY(easeTypeHeight);
    easeTypeRadio->getRect()->setY(easeTypeHeight+20);
    easeTypeSpacer->getRect()->setY(easeTypeHeight+125);
}

void ModelMapper::setupGUI(){
    cameraNames.push_back("1");cameraNames.push_back("2");cameraNames.push_back("3");
    
    adjustModes.push_back("Camera Position"); adjustModes.push_back("Camera Orientation"); adjustModes.push_back("Viewport Position");
    adjustModes.push_back("Meshes"); adjustModes.push_back("Masks");
    
    easeMethods.push_back("Linear");
    easeMethods.push_back("Quadratic");
    easeMethods.push_back("Cubic");
    easeMethods.push_back("Quartic");
    easeMethods.push_back("Exponential");
    easeMethods.push_back("Sine");
    easeMethods.push_back("Circular");
    
    positionX=NULL;
    positionY=NULL;
    positionZ=NULL;
    setMainGUI();
    setPositionGUI();
    setOrientationGUI();
    setViewportGUI();
    setMeshGUI();
    setMagnetGUI();
}

float ModelMapper::magnetMap(float distance, float radius){
    float modifier;
    
    if (easeMode==EASE_MODE_NONE){
        modifier=1;
    }
    else {
        cout<<"mode"<<endl;
        ofxTween::ofxEasingType easingType;
        int currentEase;
        
        if(easeMode==EASE_MODE_IN){
            easingType=ofxTween::easeIn;
            currentEase=easeInStyle;
        }
        else if(easeMode==EASE_MODE_OUT){
            easingType=ofxTween::easeOut;
            currentEase=easeOutStyle;
        }
        else if(easeMode==EASE_MODE_BOTH){
            easingType=ofxTween::easeInOut;
            currentEase=easeBothStyle;
        }
        
        else if(easeMode==EASE_MODE_SEPARATE){
            if(distance>magnetRadius/2){
                easingType=ofxTween::easeOut;
                currentEase=easeOutStyle;
            }
            else{
                easingType=ofxTween::easeIn;
                currentEase=easeInStyle;
            }
        }
        
        if(currentEase==EASE_STYLE_LINEAR){
            cout<<"LINEAR"<<endl;
            modifier=ofxTween::map(distance, 0, radius, 1, 0, false, easeLinear, easingType);
        }
        else if(currentEase==EASE_STYLE_QUAD){
            modifier=ofxTween::map(distance, 0, radius, 1, 0, false, easeQuad, easingType);
        }
        else if(currentEase==EASE_STYLE_CUBIC){
            modifier=ofxTween::map(distance, 0, radius, 1, 0, false, easeCubic, easingType);
        }
        else if(currentEase==EASE_STYLE_QUART){
            modifier=ofxTween::map(distance, 0, radius, 1, 0, false, easeQuart, easingType);
        }
        else if(currentEase==EASE_STYLE_EXPO){
            modifier=ofxTween::map(distance, 0, radius, 1, 0, false, easeExpo, easingType);
        }
        else if(currentEase==EASE_STYLE_SINE){
            modifier=ofxTween::map(distance, 0, radius, 1, 0, false, easeSine, easingType);
        }
        else if(currentEase==EASE_STYLE_CIRC){
            modifier=ofxTween::map(distance, 0, radius, 1, 0, false, easeCirc, easingType);
        }
        
    }
    
    return modifier;

}