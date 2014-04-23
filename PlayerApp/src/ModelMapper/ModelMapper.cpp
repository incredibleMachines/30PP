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
    bGuiCamAdjust=false;
    
    //mesh settings
    currentMesh=0;
    adjustMode=ADJUST_MODE_LOCKED;
    meshType=MESH_MASS;
    selectMode=SELECT_MODE_POINTER;
    easeMode=EASE_MODE_NONE;
    easeInStyle=EASE_STYLE_LINEAR;
    easeOutStyle=EASE_STYLE_LINEAR;
    easeBothStyle=EASE_STYLE_LINEAR;
    
    //mouse settings
    bMouseDown=false;
    clickThreshold=4;
    
    //timer for checking for double click
    mouseTimer=ofGetElapsedTimeMillis();
    
    //mask settings
    bNewMask=false;
    bDrawingMask=false;
    bMaskPoint=false;
    
    //draw settings
    bDrawWireframe=false;
    
    //adjustment settings
    bShiftPressed=false;
    moveModifier=1;
    
    //transition settings
    transitionTime=1000;
    loadTime=750;
    ofLoadImage(ambientGradientFrame,"Ambient_Frame.jpg");
    ofLoadImage(artsFrame,"Arts_Frame.jpg");
    ofLoadImage(gastronomyFrame,"Gastronomy_Frame.jpg");
    ofLoadImage(leisureFrame,"Leisure_Frame.jpg");
    ofLoadImage(shoppingFrame,"Shopping_Frame.jpg");
    ofLoadImage(marketsFrame,"Markets_Frame.jpg");
    ofLoadImage(endFrame,"End_Frame.jpg");
    bTransitioning=false;
    bTransitionStarted=false;
    bTransitionFinished=false;
    bTransitionLoading=false;
    
    //----------LOAD settings JSON
    
    ofBuffer buffer = ofBufferFromFile("settings.json");
    
    cout<< "buffer.getText.length(): "<< buffer.getText().length() <<endl;
    
    Json::Reader reader;
    if(reader.parse(buffer.getText(), settings, false)){
        cout<<"Number of Cameras: "+ofToString(settings["cameras"].size())<<endl;
        setupCameras();
    }
    
    else {
        cout  << "Failed to parse JSON: " <<  reader.getFormatedErrorMessages() << endl;
	}
    
    //setup all GUIs
    setupGUI();
    setGUIVisible(false);
    
    //----------SETUP LISTENERS
    //Event Listeners for key and mouse events
    ofAddListener(ofEvents().keyPressed,this,&ModelMapper::keyPressed);
    ofAddListener(ofEvents().keyReleased,this,&ModelMapper::keyReleased);
    ofAddListener(ofEvents().mousePressed,this,&ModelMapper::mousePressed);
    ofAddListener(ofEvents().mouseDragged,this,&ModelMapper::mouseDragged);
    ofAddListener(ofEvents().mouseReleased,this,&ModelMapper::mouseReleased);
    ofAddListener(ofEvents().mouseMoved,this,&ModelMapper::mouseMoved);
}

void ModelMapper::update(ofTexture * tex){
    
    //update gui camera to display selected camera graphics
    texture=tex;
    
    //set guiCam to have same settings as cameraSelect (less vital in three camera mode as currently setup)
    cameras[guiCam].which=cameras[cameraSelect].which;
    
    for(int i=0;i<numMeshes;i++){
        cameras[guiCam].mesh[i]=cameras[cameraSelect].mesh[i];
        cameras[guiCam].meshObjects[i]=cameras[cameraSelect].meshObjects[i];
        cameras[guiCam].meshObjects[i]=cameras[cameraSelect].meshObjects[i];
    }
    
    cameras[guiCam].masks=cameras[cameraSelect].masks;
    cameras[guiCam].highlightMask=cameras[cameraSelect].highlightMask;
    
    if(bGuiCamAdjust==false){
        cameras[guiCam].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation());
        cameras[guiCam].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition());
    }
    
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
            
            else if(adjustMode==ADJUST_MODE_2D){
                adjust2D(0,-moveModifier);
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
            else if(adjustMode==ADJUST_MODE_2D){
                adjust2D(0,moveModifier);
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
            else if(adjustMode==ADJUST_MODE_2D){
                adjust2D(moveModifier,0);
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
            else if(adjustMode==ADJUST_MODE_2D){
                adjust2D(-moveModifier,0);
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
                    setGUIVisible(false);
                }
                else{
                    setGUIVisible(true);
                }
            }
            break;
         
            
            //----------TOGGLE MESH WIREFRAME DRAW
            
        case 'w':
            if(adjustMode!=ADJUST_MODE_LOCKED){
                bDrawWireframe=!bDrawWireframe;
            }
            break;
            
            //----------TRIGGER ADJUST_MODE_LOCKED
            
        case ' ':
            if(adjustMode!=ADJUST_MODE_LOCKED){
                bDrawWireframe=false;
                wireframeButton->setColorFill(OFX_UI_COLOR_BACK);
                adjustMode=ADJUST_MODE_LOCKED;
                setGUIVisible(false);
            }
            else if(bShiftPressed==true){
                adjustMode=ADJUST_MODE_CAMERA;
                setGUIVisible(true);
                currentMode->activateToggle("Camera Position");
                performanceButton->setColorFill(OFX_UI_COLOR_BACK);;
            }
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
    
    //reset modifiers
    if(bShiftPressed==false&&bCommandPressed==false){
        moveModifier=1;
    }
    
}

void ModelMapper::mouseDragged(ofMouseEventArgs& args){
    
    //set global mouse value
    mouse=ofVec2f(args.x,args.y);
    
    //----------SELECT MESH ADJUSTMENT POINTS
    
    if(adjustMode==ADJUST_MODE_MESH){
        
        if(selectMode==SELECT_MODE_POINTER){
            selectRect=ofRectangle(mouseDown,ofVec2f(args.x,args.y));
            tempVertices=moveVertices;
            //clear point vector
            moveVertices.clear();
            
            //TODO: add shift-drag to deselect functionality?
            for(int i=0;i<numMeshes;i++)
            {
                bool drawMesh=false;
                for(int j=0;j<cameras[cameraSelect].which.size();j++){
                    if(cameras[cameraSelect].which[j]==i){
                        drawMesh=true;
                    }
                }
                
                vector<meshVertex> tempVector;
                if(drawMesh==true){
                    
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
        
        else if (selectMode==SELECT_MODE_PAINTBRUSH){
            //clear selected vertex vector
            tempVertices=moveVertices;
            moveVertices.clear();
            
            for(int i=0;i<numMeshes;i++)
            {
                bool drawMesh=false;
                for(int j=0;j<cameras[cameraSelect].which.size();j++){
                    if(cameras[cameraSelect].which[j]==i){
                        drawMesh=true;
                    }
                }
                
                vector<meshVertex> tempVector;
                
                if(drawMesh==true){
                    
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
                        else if(distance < brushRadius) {
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
                }
                
                //shift-click vector of save selected points
                if(tempVertices.size()>0){
                    for(int j=0;j < tempVertices[i].size();j++){
                        tempVector.push_back(tempVertices[i][j]);
                    }
                }
                
                moveVertices.push_back(tempVector);
                tempVector.clear();
                
                
            }
            
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
        
        if(selectMode==SELECT_MODE_PEN||selectMode==SELECT_MODE_DOUBLE_PEN){
            
            if(bDrawingPen==true){
                if(ofGetElapsedTimeMillis()-mouseTimer>250){
                    mouseTimer=ofGetElapsedTimeMillis();
                    if(bNewPen==true){
                        penPoly.addVertex(mouse);
                        bNewPen=false;
                    }
                    else{
                        penPoly.lineTo(mouse);
                    }
                }
                //double-click to finish mask
                else{
                    cout<<"DOUBLE"<<endl;
                    if(bNewPen==true){
                        bNewPen=false;
                        penPoly.clear();
                        bDrawingPen=false;
                    }
                    
                    else{
                        penPoly.close();
                        bDrawingPen=false;
                        
                        tempVertices=moveVertices;
                        moveVertices.clear();
                        
                        for(int i=0;i<numMeshes;i++)
                        {
                            bool drawMesh=false;
                            for(int j=0;j<cameras[cameraSelect].which.size();j++){
                                if(cameras[cameraSelect].which[j]==i){
                                    drawMesh=true;
                                }
                            }
                            
                            vector<meshVertex> tempVector;
                            if(drawMesh==true){
                                
                                //determine vertices inside drag box on gui camera screen
                                for(int j = 0; j < cameras[guiCam].mesh[i].getNumVertices(); j++){
                                    ofVec3f cur = cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(j),cameras[guiCam].viewport);
                                    if(penPoly.inside(cur)) {
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
                        penPoly.clear();
                        penButton->setColorFill(OFX_UI_COLOR_BACK);
                        calculateMagnetPoints();
                    }
                }
            }
            
            if (selectMode==SELECT_MODE_DOUBLE_PEN){
                if(bDrawingDouble==true){
                    if(ofGetElapsedTimeMillis()-mouseTimer>250){
                        mouseTimer=ofGetElapsedTimeMillis();
                        if(bNewDouble==true){
                            doublePoly.addVertex(mouse);
                            bNewDouble=false;
                        }
                        else{
                            doublePoly.lineTo(mouse);
                        }
                    }
                    
                    //double-click to finish mask
                    else{
                        cout<<"DOUBLE"<<endl;
                        if(bNewDouble==true){
                            bNewDouble=false;
                            doublePoly.clear();
                            bDrawingDouble=false;
                        }
                        
                        else{
                            doublePoly.close();
                            bDrawingDouble=false;
                            
                            tempDoubleVertices=doubleVertices;
                            doubleVertices.clear();
                            
                            for(int i=0;i<numMeshes;i++)
                            {
                                bool drawMesh=false;
                                for(int j=0;j<cameras[cameraSelect].which.size();j++){
                                    if(cameras[cameraSelect].which[j]==i){
                                        drawMesh=true;
                                    }
                                }
                                
                                vector<meshVertex> tempVector;
                                
                                if(drawMesh==true){
                                    
                                    //determine vertices inside drag box on gui camera screen
                                    for(int j = 0; j < cameras[guiCam].mesh[i].getNumVertices(); j++){
                                        ofVec3f cur = cameras[guiCam].camera.worldToScreen(cameras[guiCam].mesh[i].getVertex(j),cameras[guiCam].viewport);
                                        if(doublePoly.inside(cur)) {
                                            meshVertex tempVert;
                                            tempVert.vertex=cur;
                                            tempVert.index=j;
                                            bool included=false;
                                            if(tempDoubleVertices.size()>0){
                                                for(int k=0;k<tempDoubleVertices[i].size();k++){
                                                    if(tempDoubleVertices[i][k].index==tempVert.index){
                                                        included=true;
                                                        if(bShiftPressed==true){
                                                            tempDoubleVertices[i].erase(tempDoubleVertices[i].begin()+k);
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
                                }
                                
                                //push stored points into vector (to enable shift select command)
                                if(tempDoubleVertices.size()>0){
                                    for(int j=0;j < tempDoubleVertices[i].size();j++){
                                        tempVector.push_back(tempDoubleVertices[i][j]);
                                    }
                                }
                                
                                doubleVertices.push_back(tempVector);
                                tempVector.clear();
                            }
                        }
                        doublePoly.clear();
                        doublePenButton->setColorFill(OFX_UI_COLOR_BACK);
                        calculateMagnetPoints();
                    }
                }
            }
        }
        
        else if (selectMode==SELECT_MODE_POINTER){
            //clear selected vertex vector
            tempVertices=moveVertices;
            moveVertices.clear();
            
            for(int i=0;i<numMeshes;i++)
            {
                bool drawMesh=false;
                for(int j=0;j<cameras[cameraSelect].which.size();j++){
                    if(cameras[cameraSelect].which[j]==i){
                        drawMesh=true;
                    }
                }
                
                vector<meshVertex> tempVector;
                
                if(drawMesh==true){
                    
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
                }
                
                //shift-click vector of save selected points
                if(tempVertices.size()>0){
                    for(int j=0;j < tempVertices[i].size();j++){
                        tempVector.push_back(tempVertices[i][j]);
                    }
                }
                
                moveVertices.push_back(tempVector);
                tempVector.clear();
                
                
            }
        }
        
        else if (selectMode==SELECT_MODE_PAINTBRUSH){
            //clear selected vertex vector
            tempVertices=moveVertices;
            moveVertices.clear();
            
            for(int i=0;i<numMeshes;i++)
            {
                bool drawMesh=false;
                for(int j=0;j<cameras[cameraSelect].which.size();j++){
                    if(cameras[cameraSelect].which[j]==i){
                        drawMesh=true;
                    }
                }
                
                vector<meshVertex> tempVector;
                if(drawMesh==true){
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
                        else if(distance < brushRadius) {
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
                }
                
                //shift-click vector of save selected points
                if(tempVertices.size()>0){
                    for(int j=0;j < tempVertices[i].size();j++){
                        tempVector.push_back(tempVertices[i][j]);
                    }
                }
                
                moveVertices.push_back(tempVector);
                tempVector.clear();
                
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
                maskButton->setColorFill(OFX_UI_COLOR_BACK);
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
    
    else if (adjustMode==ADJUST_MODE_2D){
        
        tempVertices2D=vertices2D;
        vertices2D.clear();
        vector<vertex2D> tempVector;
        
        ofVec3f nearestVertex;
        float drawNearest=false;
        for(int i=0;i<numMeshes;i++){
            bool drawMesh=false;
            for(int j=0;j<cameras[cameraSelect].which.size();j++){
                if(cameras[cameraSelect].which[j]==i){
                    drawMesh=true;
                }
            }
            
            if(drawMesh==true){
                if(cameras[guiCam].meshObjects[i].isMesh==false){
                    float nearestDistance = 15;
                    //parse through all vertices to determine nearest, if exists
                    for(int j = 0; j < cameras[guiCam].meshObjects[i].warped.size(); j++) {
                        for(int k = 0; k < cameras[guiCam].meshObjects[i].warped[j].size(); k++){
                            float distance = cameras[guiCam].meshObjects[i].warped[j][k].distance((mouse-cameras[guiCam].meshObjects[i].translate)/cameras[guiCam].meshObjects[i].scale);
                            if(distance < nearestDistance) {
                                nearestVertex = cameras[guiCam].meshObjects[i].warped[j][k]*cameras[guiCam].meshObjects[i].scale+cameras[guiCam].meshObjects[i].translate;
                                
                                vertex2D tempVert;
                                tempVert.vertex=nearestVertex;
                                tempVert.index=k;
                                tempVert.box=j;
                                bool included=false;
                                if(tempVertices2D.size()>0){
                                    for(int l=0;l<tempVertices2D[i].size();l++){
                                        if(tempVertices2D[i][l].box==tempVert.box&&tempVertices2D[i][l].index==tempVert.index){
                                            included=true;
                                            if(bShiftPressed==true){
                                                tempVertices2D[i].erase(tempVertices2D[i].begin()+l);
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
                        
                    }
                }
            }
            
            
            //vector of save selected points
            if(tempVertices2D.size()>0){
                for(int j=0;j < tempVertices2D[i].size();j++){
                    tempVector.push_back(tempVertices2D[i][j]);
                }
            }
            
            vertices2D.push_back(tempVector);
            tempVector.clear();
        }
    }
    mouse=ofVec2f(args.x,args.y);
}


//--------------------------------------------------------------
void ModelMapper::mouseReleased(ofMouseEventArgs& args){
    //reset drag box status
    if(bMouseDown==true){
        bMouseDown=false;
        if(adjustMode==ADJUST_MODE_MESH){
            if(selectMode==SELECT_MODE_POINTER||selectMode==SELECT_MODE_PAINTBRUSH){
                calculateMagnetPoints();
            }
        }
    }
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
        
        //save 2D meshes
        for(int j=0; j<numMeshes;j++){
            
            settings["cameras"][i]["meshes"][j]["translate"]["x"] = cameras[i].meshObjects[j].translate.x;
            settings["cameras"][i]["meshes"][j]["translate"]["y"] = cameras[i].meshObjects[j].translate.y;
            settings["cameras"][i]["meshes"][j]["scale"] = cameras[i].meshObjects[j].scale;
            settings["cameras"][i]["meshes"][j]["feather"]["right"] = cameras[i].meshObjects[j].right;
            settings["cameras"][i]["meshes"][j]["feather"]["left"] = cameras[i].meshObjects[j].left;
            
            settings["cameras"][i]["meshes"][j]["grid"]["x"] = cameras[i].meshObjects[j].horizGrid;
            settings["cameras"][i]["meshes"][j]["grid"]["y"] = cameras[i].meshObjects[j].vertGrid;
            
            settings["cameras"][i]["meshes"][j]["texPos"]["x"] = cameras[i].meshObjects[j].tex.pos.x;
            settings["cameras"][i]["meshes"][j]["texPos"]["y"] = cameras[i].meshObjects[j].tex.pos.y;
            
            settings["cameras"][i]["meshes"][j]["texWidth"] = cameras[i].meshObjects[j].tex.width;
            settings["cameras"][i]["meshes"][j]["texHeight"] = cameras[i].meshObjects[j].tex.height;
            
            
            for(int k=0; k<cameras[i].meshObjects[j].warped.size(); k++){
                for(int l=0; l<cameras[i].meshObjects[j].warped[k].size();l++){
                    settings["cameras"][i]["meshes"][j]["warped"][k][l]["x"]=cameras[i].meshObjects[j].warped[k][l].x;
                    settings["cameras"][i]["meshes"][j]["warped"][k][l]["y"]=cameras[i].meshObjects[j].warped[k][l].y;
                }
            }
        }
        
        //----------SAVE INDIVIDUAL 3D MESH
        
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
        cout<<"settings.json SAVED"<<endl;
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
    
    //----------SAVE JSON settings DOCUMENT
    
    ofFile file;
    if (!file.open("settings.json", ofFile::WriteOnly)) {
        cout<<"ERROR: ofxJSONElement::save" << "Unable to open " << file.getAbsolutePath() << "."<<endl;
    }
    else {
        Json::StyledWriter writer;
        file << writer.write(settings) << endl;
        cout<<"settings.json SAVED"<<endl;
        file.close();
    }
}

void ModelMapper:: drawCameras() {
    
    for(int i = 0; i < numCams; i++){
        
        if(adjustMode!=ADJUST_MODE_LOCKED||i!=guiCam){
            
            //CREATE AND POPULATE CAMERA
            
            //Begin camera object
            cameras[i].camera.begin(cameras[i].viewport);
            
            
            //DRAW 3D MESHES
            
            for(int j=0;j<numMeshes;j++){
                
                bool drawMesh=false;
                for(int k=0;k<cameras[i].which.size();k++){
                    if(cameras[i].which[k]==j){
                        drawMesh=true;
                    }
                }
                
                if(drawMesh==true){
                    
                    if(cameras[i].meshObjects[j].isMesh==true){
                        ofEnableNormalizedTexCoords();
                        texture->bind();
                        
                        ofSetColor(255,255,255);
                        
                        cameras[i].mesh[j].draw();
                        
                        texture->unbind();
                        
                        //draw mesh wireframe
                        if(bDrawWireframe==true){
                            ofSetColor(255,255,255,100);
                            ofSetLineWidth(2);
                            cameras[i].mesh[j].drawWireframe();
                        }
                    }
                }
            }
            
            
            //End camera object
            cameras[i].camera.end();
            
            
            //DRAW 2D MESHES
            
            for(int j=0;j<numMeshes;j++){
                
                bool drawMesh=false;
                for(int k=0;k<cameras[i].which.size();k++){
                    if(cameras[i].which[k]==j){
                        drawMesh=true;
                    }
                }
                
                if(drawMesh==true){
                    
                    if(cameras[i].meshObjects[j].isMesh==false){
                        for(int k=0; k<cameras[i].meshObjects[j].originals.size();k++){
                            ofEnableAlphaBlending();
                            ofDisableNormalizedTexCoords();
                            ofMatrix4x4 homography = ofxHomography::findHomography(cameras[i].meshObjects[j].originals[k], cameras[i].meshObjects[j].warped[k]);
                            
                            ofPushMatrix();
                            ofSetColor(255,255,255);
                            
                            ofTranslate(cameras[i].meshObjects[j].translate+ofPoint(cameras[i].viewport.x,cameras[i].viewport.y));
                            ofScale(cameras[i].meshObjects[j].scale,cameras[i].meshObjects[j].scale);
                            
                            ofPushMatrix();
                            glMultMatrixf(homography.getPtr());
                            if(i!=guiCam){
                                glDepthFunc(GL_LESS);
                                //draw UV Section
                                ofSetColor(255,255,255,255);
                                texture->drawSubsection(cameras[i].meshObjects[j].originals[k][0].x,
                                                        cameras[i].meshObjects[j].originals[k][0].y,
                                                        cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                        cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1),
                                                        cameras[i].meshObjects[j].tex.pos.x+cameras[i].meshObjects[j].originals[k][0].x,
                                                        cameras[i].meshObjects[j].tex.pos.y+cameras[i].meshObjects[j].originals[k][0].y,
                                                        cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                        cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1));
                                
                                //draw Transition image on top
                                if(bTransitioning==true){
                                    glDepthFunc(GL_ALWAYS);
                                    if(bTransitionStarted==true){
                                        if(ofGetElapsedTimeMillis()-transitionTimer>transitionTime){
                                            if(i==numCams-1)bTransitionStarted=false;
                                            ofSetColor(255,255,255,255);
                                        }
                                        int opacity=int(ofMap(ofGetElapsedTimeMillis()-transitionTimer,0,transitionTime,0,255));
                                        if(opacity>255) opacity=255;
                                            ofSetColor(255,255,255,opacity);
   
                                            fadeFrame->drawSubsection(cameras[i].meshObjects[j].originals[k][0].x,
                                                                      cameras[i].meshObjects[j].originals[k][0].y,
                                                                      cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                                      cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1),
                                                                      cameras[i].meshObjects[j].tex.pos.x+cameras[i].meshObjects[j].originals[k][0].x,
                                                                      cameras[i].meshObjects[j].tex.pos.y+cameras[i].meshObjects[j].originals[k][0].y,
                                                                      cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                                      cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1));
                                    }
                                    
                                    else if(bTransitionLoading==true){
                                        if(ofGetElapsedTimeMillis()-transitionTimer>loadTime){
                                                bTransitionFinished=true;
                                                bTransitionLoading=false;

                                            transitionTimer=ofGetElapsedTimeMillis();
                                            ofSetColor(255,255,255,255);
                                        }
                                            ofSetColor(255,255,255,255);
                                            fadeFrame->drawSubsection(cameras[i].meshObjects[j].originals[k][0].x,
                                                                      cameras[i].meshObjects[j].originals[k][0].y,
                                                                      cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                                      cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1),
                                                                      cameras[i].meshObjects[j].tex.pos.x+cameras[i].meshObjects[j].originals[k][0].x,
                                                                      cameras[i].meshObjects[j].tex.pos.y+cameras[i].meshObjects[j].originals[k][0].y,
                                                                      cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                                      cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1));
                                    }
                                    
                                    else if(bTransitionFinished==true){
                                        if(ofGetElapsedTimeMillis()-transitionTimer>transitionTime){
                                                bTransitionFinished=false;
                                                bTransitioning=false;
                                        }
                                            ofSetColor(255,255,255,int(ofMap(ofGetElapsedTimeMillis()-transitionTimer,0,transitionTime,255,0)));
                                        
                                            fadeFrame->drawSubsection(cameras[i].meshObjects[j].originals[k][0].x,
                                                                      cameras[i].meshObjects[j].originals[k][0].y,
                                                                      cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                                      cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1),
                                                                      cameras[i].meshObjects[j].tex.pos.x+cameras[i].meshObjects[j].originals[k][0].x,
                                                                      cameras[i].meshObjects[j].tex.pos.y+cameras[i].meshObjects[j].originals[k][0].y,
                                                                      cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1),
                                                                      cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1));
                                    }
                                    glDepthFunc(GL_LESS);
                                }
                                
                            }
                            
                            // software edge blending
                            //                            glDepthFunc(GL_ALWAYS);
                            //                            for(int l=0; l<cameras[i].meshObjects[j].left; l++){
                            //                                ofSetLineWidth(1);
                            //                                ofSetColor(0,0,0,ofMap(l,0,cameras[i].meshObjects[j].left,255,0));
                            //                                ofLine(cameras[i].meshObjects[j].originals[k][0].x+l,cameras[i].meshObjects[j].originals[k][0].y,cameras[i].meshObjects[j].originals[k][0].x+l,cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1));
                            //                            }
                            //
                            //                            for(int l=0; l<cameras[i].meshObjects[j].right; l++){
                            //                                ofSetLineWidth(1);
                            //                                ofSetColor(0,0,0,ofMap(l,0,cameras[i].meshObjects[j].right,255,0));
                            //                                ofLine(cameras[i].meshObjects[j].originals[k][0].x+cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1)-l,cameras[i].meshObjects[j].originals[k][0].y,cameras[i].meshObjects[j].originals[k][0].x+cameras[i].meshObjects[j].tex.width/(cameras[i].meshObjects[j].horizGrid-1)-l,cameras[i].meshObjects[j].tex.height/(cameras[i].meshObjects[j].vertGrid-1));
                            //                            }
                            //                            glDepthFunc(GL_LESS);
                            
                            
                            if(bDrawWireframe==true){
                                ofSetColor(0,255,0);
                                ofNoFill();
                                glDepthFunc(GL_ALWAYS);
                                ofRect(cameras[i].meshObjects[j].originals[k][0].x,cameras[i].meshObjects[j].originals[k][0].y,cameras[i].meshObjects[j].originals[k][1].x-cameras[i].meshObjects[j].originals[k][0].x,cameras[i].meshObjects[j].originals[k][2].y-cameras[i].meshObjects[j].originals[k][0].y);
                                glDepthFunc(GL_LESS);
                            }
                            ofPopMatrix();
                            ofPopMatrix();
                            
                            ofDisableAlphaBlending();
                        }
                    }
                }
            }
        }
        
        //ADJUST_MODE_LOCKED guiCam text
        //        else{
        //            ofDrawBitmapString("Presentation Mode Active. Press Shift + Spacebar to unlock and edit", cameras[guiCam].viewport.x+cameras[guiCam].viewport.width/2-300, cameras[guiCam].viewport.y+cameras[guiCam].viewport.height/2);
        //        }
        
        
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
        if(selectMode==SELECT_MODE_PEN||selectMode==SELECT_MODE_DOUBLE_PEN){
            if(bShiftPressed==true){
                ofSetColor(0);
            }
            else{
                ofSetColor(255,0,0);
            }
            ofSetLineWidth(1);
            penPoly.draw();
            ofSetColor(255);
        }
        
        if(selectMode==SELECT_MODE_DOUBLE_PEN){
            if(bShiftPressed==true){
                ofSetColor(0);
            }
            else{
                ofSetColor(255);
            }
            ofSetLineWidth(1);
            doublePoly.draw();
            ofSetColor(255);
        }
        
        
        //----------DRAW NEAREST VERTEX
        
        //determine nearest vertex and highlight yellow, if within clickThreshold distance of vertex
        for(int i=0;i<numMeshes;i++){
            bool drawMesh=false;
            for(int j=0;j<cameras[cameraSelect].which.size();j++){
                if(cameras[cameraSelect].which[j]==i){
                    drawMesh=true;
                }
            }
            
            if(drawMesh==true){
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
                
                ofNoFill();
                ofSetLineWidth(1);
                
                if(selectMode==SELECT_MODE_POINTER||selectMode==SELECT_MODE_PAINTBRUSH){
                    //draw nearest highlight circle on both gui camera and selected camera
                    if(drawNearest==true){
                        ofSetColor(ofColor::yellow);
                        ofCircle(nearestVertex, 4);
                        ofCircle(cameras[cameraSelect].camera.worldToScreen(cameras[cameraSelect].mesh[i].getVertex(nearestIndex),cameras[cameraSelect].viewport),4);
                    }
                }
                
                //----------DRAW SELECTED VERTICES
                
                //draw selected circles on all vertices selected by clicking or via drag box on both gui camera and selected camera, set in mousePressed() and mouseDragged()
                ofSetColor(255,0,0);
                if(moveVertices.size()>0){
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
                
                if(magnetVertices.size()>0){
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
            }
            
            
        }
    }
    
    else if(adjustMode==ADJUST_MODE_MASK){
        
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
    
    else if (adjustMode==ADJUST_MODE_2D){
        ofVec3f nearestVertex;
        float drawNearest=false;
        for(int i=0;i<numMeshes;i++){
            bool drawMesh=false;
            for(int j=0;j<cameras[cameraSelect].which.size();j++){
                if(cameras[cameraSelect].which[j]==i){
                    drawMesh=true;
                }
            }
            
            if(drawMesh==true){
                
                if(cameras[guiCam].meshObjects[i].isMesh==false){
                    float nearestDistance = 15;
                    
                    //parse through all vertices to determine nearest, if exists
                    for(int j = 0; j < cameras[guiCam].meshObjects[i].warped.size(); j++) {
                        for(int k = 0; k < cameras[guiCam].meshObjects[i].warped[j].size(); k++){
                            float distance = cameras[guiCam].meshObjects[i].warped[j][k].distance((mouse-cameras[guiCam].meshObjects[i].translate)/cameras[guiCam].meshObjects[i].scale);
                            if(distance < nearestDistance) {
                                nearestDistance = distance;
                                nearestVertex = cameras[guiCam].meshObjects[i].warped[j][k]*cameras[guiCam].meshObjects[i].scale+cameras[guiCam].meshObjects[i].translate;
                                drawNearest=true;
                            }
                        }
                    }
                }
                
                if(vertices2D.size()>0){
                    for(int j=0;j<vertices2D[i].size();j++){
                        ofSetColor(0,255,0);
                        ofNoFill();
                        ofSetLineWidth(2);
                        ofCircle(vertices2D[i][j].vertex.x+cameras[guiCam].viewport.x,vertices2D[i][j].vertex.y+cameras[guiCam].viewport.y,4);
                        if(cameraSelect!=guiCam){
                            ofCircle(vertices2D[i][j].vertex.x+cameras[cameraSelect].viewport.x,vertices2D[i][j].vertex.y+cameras[cameraSelect].viewport.y,4);
                        }
                    }
                }
            }
        }
        
        
        
        //draw nearest highlight circle on both gui camera and selected camera
        ofNoFill();
        ofSetLineWidth(2);
        if(drawNearest==true){
            ofSetColor(0,255,0);
            ofCircle(nearestVertex, 4);
        }
    }
    
    if(adjustMode==ADJUST_MODE_MESH&&selectMode==SELECT_MODE_PAINTBRUSH){
        ofSetColor(255);
        ofCircle(mouse, brushRadius);
    }
    
    //reset line width
    ofSetLineWidth(1);
}

void ModelMapper::drawMasks(){
    //draw mask ofPaths
    for(int i = 0; i < numCams; i++){
        for(int j=cameras[i].drawMasks.size()-1; j>=0;j--){
            if(i!=guiCam||bGuiCamAdjust==false){
                
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
    
    if(bGuiCamAdjust==true){
        cameras[guiCam].camera.setGlobalPosition(cameras[guiCam].camera.getGlobalPosition()+ofVec3f(x,y,z));
        
    }
    else{
        cameras[cameraSelect].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition()+ofVec3f(x,y,z));
    }
    
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
    if(bGuiCamAdjust==true){
        cameras[guiCam].camera.setGlobalOrientation(cameras[guiCam].camera.getGlobalOrientation() *= yRot*xRot*zRot);
    }
    else{
        cameras[cameraSelect].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation() *= yRot*xRot*zRot);
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

void ModelMapper::adjust2D(float x, float y){
    
    for(int i=0;i<vertices2D.size();i++){
        for(int j=0;j<vertices2D[i].size();j++){
            cameras[cameraSelect].meshObjects[i].warped[vertices2D[i][j].box][vertices2D[i][j].index].x+=x;
            vertices2D[i][j].vertex.x+=x*cameras[cameraSelect].meshObjects[i].scale;
            cameras[cameraSelect].meshObjects[i].warped[vertices2D[i][j].box][vertices2D[i][j].index].y+=y;
            vertices2D[i][j].vertex.y+=y*cameras[cameraSelect].meshObjects[i].scale;
        }
    }
    
}

void ModelMapper::calculateMagnetPoints(){
    
    cout<<"CALCULATE MAGNET POINTS"<<endl;
    magnetVertices.clear();
    
    if(selectMode!=SELECT_MODE_DOUBLE_PEN){
        for(int i=0;i<moveVertices.size();i++){
            
            vector<meshVertex> tempVector;
            
            for(int j=0;j<moveVertices[i].size();j++){
                
                int n = cameras[guiCam].mesh[i].getNumVertices();
                
                //parse through all vertices to determine nearest, if exists
                for(int k = 0; k < n; k++) {
                    
                    ofVec3f check = cameras[guiCam].mesh[i].getVertex(k);
                    ofVec3f cur=cameras[guiCam].mesh[i].getVertex(moveVertices[i][j].index);
                    
                    float distance = abs(check.distance(cur));
                    float maxDist = 0;
                    
                    if (distance<magnetRadius&&distance!=0){
                        meshVertex tempVert;
                        bool duplicate=false;
                        bool selected=false;
                        
                        for(int l=0;l<moveVertices[i].size();l++){
                            if(moveVertices[i][l].index==k){
                                selected=true;
                                //break;
                            }
                        }
                        
                        if(selected==false){
                            for(int l=0;l<tempVector.size();l++){
                                if(tempVector[l].index==k){
                                    duplicate=true;
                                    if(distance<tempVector[l].distance){
                                        tempVector[l].distance=distance;
                                        tempVector[l].modifier=magnetMap(distance, magnetRadius);
                                    }
                                    //break;
                                }
                            }
                        }
                        
                        if(duplicate==false&&selected==false){
                            tempVert.vertex=cur;
                            tempVert.index=k;
                            tempVert.distance=distance;
                            tempVert.modifier=magnetMap(distance, magnetRadius);
                            tempVector.push_back(tempVert);
                        }
                    }
                }
            }
            magnetVertices.push_back(tempVector);
            tempVector.clear();
        }
    }
    
    else if (selectMode==SELECT_MODE_DOUBLE_PEN){
        for(int i=0;i<doubleVertices.size();i++){
            
        }
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
    mainGUI->addToggle("Adjust GUI Separately", false);
    
    mainGUI->addSpacer();
    mainGUI->addLabel("Adjust:",OFX_UI_FONT_MEDIUM);
    currentMode=mainGUI->addRadio("CHANGE MODE", adjustModes, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    currentMode->activateToggle("Camera Position");
    
    mainGUI->addSpacer();
    wireframeButton=mainGUI->addLabelButton("TOGGLE WIREFRAME", false);
    wireframeButton->setColorFill(OFX_UI_COLOR_BACK);
    
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
    magnetMethods.push_back("Paintbrush");
    magnetMethods.push_back("Pen");
    magnetMethods.push_back("Double Pen");
    
    meshGUI->addSpacer();
    meshGUI->addLabel("Selection:",OFX_UI_FONT_MEDIUM);
    meshGUI->addRadio("SELECTION TYPE", magnetMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM)->activateToggle("Pointer");
    
    meshGUI->addSpacer();
    meshGUI->addLabelButton("CLEAR SELECTION", false);
    
    meshGUI->addSpacer();
    meshGUI->addLabelButton("RELOAD MESH", false);
    meshGUI->addLabelButton("RELOAD SELECTED", false);
    
    meshGUI->setPosition(212, 0);
    meshGUI->autoSizeToFitWidgets();
    
    meshGUI->setVisible(false);
    ofAddListener(meshGUI->newGUIEvent,this,&ModelMapper::guiEvent);
    
}

void ModelMapper::setMesh2DGUI(){
    mesh2DGUI = new ofxUISuperCanvas("Mesh 2D");
    mesh2DGUI->addSpacer();
    mesh2DGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    mesh2DGUI->addLabel("Click vertices to select", OFX_UI_FONT_SMALL);
    mesh2DGUI->addLabel("Drag over to select multiple", OFX_UI_FONT_SMALL);
    mesh2DGUI->addLabel("Shift-click to remove", OFX_UI_FONT_SMALL);
    mesh2DGUI->addLabel("(Arrow Keys) Adjust x/y", OFX_UI_FONT_SMALL);
    mesh2DGUI->addLabel("(z/a) Adjust z", OFX_UI_FONT_SMALL);
    mesh2DGUI->addLabel("(Shift) for fast move", OFX_UI_FONT_SMALL);
    mesh2DGUI->addLabel("(Cmd) for slow move", OFX_UI_FONT_SMALL);
    
    mesh2DGUI->addLabel("Translate X",OFX_UI_FONT_MEDIUM);
    translateX2D = mesh2DGUI->addTextInput("Translate X", ofToString(cameras[cameraSelect].meshObjects[currentMesh].translate.x));
    translateX2D->setAutoClear(false);
    
    mesh2DGUI->addLabel("Translate Y",OFX_UI_FONT_MEDIUM);
    translateY2D = mesh2DGUI->addTextInput("Translate Y", ofToString(cameras[cameraSelect].meshObjects[currentMesh].translate.y));
    translateY2D->setAutoClear(false);
    
    mesh2DGUI->addLabel("Scale",OFX_UI_FONT_MEDIUM);
    scale2D = mesh2DGUI->addTextInput("Scale", ofToString(cameras[cameraSelect].meshObjects[currentMesh].scale));
    scale2D->setAutoClear(false);
    
    mesh2DGUI->addLabel("Feather",OFX_UI_FONT_MEDIUM);
    featherRight2D = mesh2DGUI->addTextInput("Feather Right", ofToString(cameras[cameraSelect].meshObjects[currentMesh].right));
    featherRight2D->setAutoClear(false);
    featherLeft2D = mesh2DGUI->addTextInput("Feather Left", ofToString(cameras[cameraSelect].meshObjects[currentMesh].left));
    featherLeft2D->setAutoClear(false);
    
    mesh2DGUI->addSpacer();
    mesh2DGUI->addLabelButton("CLEAR SELECTION", false);
    mesh2DGUI->addSpacer();
    mesh2DGUI->addLabelButton("RESET WARP", false);
    
    mesh2DGUI->setPosition(212, 0);
    mesh2DGUI->autoSizeToFitWidgets();
    
    mesh2DGUI->setVisible(false);
    ofAddListener(mesh2DGUI->newGUIEvent,this,&ModelMapper::guiEvent);
    
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
    magnetRadiusLabel=magnetGUI->addLabel("Blend Amount",OFX_UI_FONT_MEDIUM);
    magnetRadiusSet = magnetGUI->addTextInput("Blend Amount", ofToString(magnetRadius));
    magnetRadiusSet->setAutoClear(false);
    
    brushRadiusLabel=magnetGUI->addLabel("Brush Radius",OFX_UI_FONT_MEDIUM);
    brushRadiusSet = magnetGUI->addTextInput("Brush Radius", ofToString(magnetRadius));
    brushRadiusSet->setAutoClear(false);
    brushRadiusLabel->setVisible(false);
    brushRadiusSet->setVisible(false);
    
    penButton=magnetGUI->addLabelButton("START SELECTION", OFX_UI_FONT_MEDIUM);
    penButton->setVisible(false);
    doublePenButton=magnetGUI->addLabelButton("START OUTER", OFX_UI_FONT_MEDIUM);
    doublePenButton->setVisible(false);
    
    magnetRadiusSpacer=magnetGUI->addSpacer();
    easeTypeLabel=magnetGUI->addLabel("Ease Type:");
    easeTypeRadio=magnetGUI->addRadio("EASE TYPE", easeTypes, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    easeTypeRadio->activateToggle("None");
    
    easeTypeSpacer=magnetGUI->addSpacer();
    easeInLabel = magnetGUI->addLabel("Ease In Method:");
    easeInMethod = magnetGUI->addRadio("EASE IN", easeMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    easeInMethod->setVisible(false);
    
    easeOutLabel = magnetGUI->addLabel("Ease Out Method:");
    easeOutMethod = magnetGUI->addRadio("EASE OUT", easeMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    easeOutMethod->setVisible(false);
    
    easeBothLabel = magnetGUI->addLabel("Ease Both Method:");
    easeBothMethod = magnetGUI->addRadio("EASE BOTH", easeMethods, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
    easeBothMethod->setVisible(false);
    
    magnetGUI->setPosition(212*2, 0);
    magnetGUI->setHeight(220);
    
    setEaseHeights(true,false,false,false);
    
    magnetGUI->setVisible(false);
    ofAddListener(magnetGUI->newGUIEvent,this,&ModelMapper::guiEvent);
}

void ModelMapper::setMaskGUI(){
    maskGUI= new ofxUISuperCanvas("Masks");
    maskGUI->addSpacer();
    maskGUI->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    maskGUI->addLabel("Click mask point to adjust", OFX_UI_FONT_SMALL);
    maskGUI->addLabel("Click inside mask to move", OFX_UI_FONT_SMALL);
    maskGUI->addLabel("(Delete) to delete", OFX_UI_FONT_SMALL);
    maskGUI->addLabel("(Arrow Keys) Adjust x/y", OFX_UI_FONT_SMALL);
    maskGUI->addLabel("(Shift) for fast move", OFX_UI_FONT_SMALL);
    maskGUI->addLabel("(Cmd) for slow move", OFX_UI_FONT_SMALL);
    
    maskGUI->addSpacer();
    maskGUI->addLabel("Click for new mask",OFX_UI_FONT_MEDIUM);
    maskGUI->addLabel("Double click to close", OFX_UI_FONT_SMALL);
    maskButton=maskGUI->addLabelButton("START MASK", OFX_UI_FONT_MEDIUM);
    maskButton->setColorFill(OFX_UI_COLOR_BACK);
    
    maskGUI->setPosition(212, 0);
    maskGUI->autoSizeToFitWidgets();
    
    maskGUI->setVisible(false);
    ofAddListener(maskGUI->newGUIEvent,this,&ModelMapper::guiEvent);
}

void ModelMapper::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    int kind = e.getKind();
    
    cout << "got event from: " << name << endl;
    
    if(name=="TOGGLE WIREFRAME"){
        if(adjustMode!=ADJUST_MODE_LOCKED){
            bDrawWireframe=!bDrawWireframe;
            if(bDrawWireframe==true){
                wireframeButton->setColorFill(OFX_UI_COLOR_FILL);
            }
            else{
                wireframeButton->setColorFill(OFX_UI_COLOR_BACK);
            }
        }
    }
    
    else if(name=="PERFORMANCE MODE"){
        bDrawWireframe=false;
        wireframeButton->setColorBack(OFX_UI_COLOR_BACK);
        performanceButton->setColorBack(OFX_UI_COLOR_BACK);
        adjustMode=ADJUST_MODE_LOCKED;
        setGUIVisible(false);
    }
    
    else if(name == "Adjust GUI Separately")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        
        if(toggle->getValue()==true){
            bGuiCamAdjust=true;
        }
        
        else{
            bGuiCamAdjust=false;
        }
    }
    
    else if(name == "Camera X")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].camera.setGlobalPosition(ofVec3f(ofToFloat(ti->getTextString()),cameras[cameraSelect].camera.getGlobalPosition().y,cameras[cameraSelect].camera.getGlobalPosition().z));
        }
    }
    
    else if(name == "Camera Y")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].camera.setGlobalPosition(ofVec3f(cameras[cameraSelect].camera.getGlobalPosition().x,ofToFloat(ti->getTextString()),cameras[cameraSelect].camera.getGlobalPosition().z));
        }
    }
    
    else if(name == "Camera Z")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].camera.setGlobalPosition(ofVec3f(cameras[cameraSelect].camera.getGlobalPosition().x,cameras[cameraSelect].camera.getGlobalPosition().y,ofToFloat(ti->getTextString())));
        }
    }
    
    else if(name == "Orientation X")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(ofToFloat(ti->getTextString()), cameras[cameraSelect].camera.getGlobalOrientation().y(), cameras[cameraSelect].camera.getGlobalOrientation().z(), cameras[cameraSelect].camera.getGlobalOrientation().w());
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);
        }
    }
    
    else if(name == "Orientation Y")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(cameras[cameraSelect].camera.getGlobalOrientation().x(),ofToFloat(ti->getTextString()), cameras[cameraSelect].camera.getGlobalOrientation().z(), cameras[cameraSelect].camera.getGlobalOrientation().w());
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);
        }
    }
    
    else if(name == "Orientation Z")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(cameras[cameraSelect].camera.getGlobalOrientation().x(), cameras[cameraSelect].camera.getGlobalOrientation().y(),ofToFloat(ti->getTextString()), cameras[cameraSelect].camera.getGlobalOrientation().w());
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);
        }
    }
    
    else if(name == "Orientation W")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofQuaternion rot = ofQuaternion(cameras[cameraSelect].camera.getGlobalOrientation().x(), cameras[cameraSelect].camera.getGlobalOrientation().y(), cameras[cameraSelect].camera.getGlobalOrientation().z(),ofToFloat(ti->getTextString()));
            
            cameras[cameraSelect].camera.setGlobalOrientation(rot);
        }
    }
    
    else if(name == "Viewport X")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].viewport.x=ofToFloat(ti->getTextString());
        }
    }
    
    else if(name == "Viewport Y")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].viewport.y=ofToFloat(ti->getTextString());
        }
    }
    
    else if(name == "Translate X")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].meshObjects[currentMesh].translate.x=ofToFloat(ti->getTextString());
        }
    }
    
    else if(name == "Translate Y")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].meshObjects[currentMesh].translate.y=ofToFloat(ti->getTextString());
        }
    }
    
    else if(name == "Scale")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].meshObjects[currentMesh].scale=ofToFloat(ti->getTextString());
        }
    }
    
    else if(name == "Feather Right")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].meshObjects[currentMesh].right=ofToFloat(ti->getTextString());
        }
    }
    
    else if(name == "Feather Left")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cameras[cameraSelect].meshObjects[currentMesh].left=ofToFloat(ti->getTextString());
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
        //        for(int i=0;i<vertices2D.size();i++){
        //            vertices2D[i].clear();
        //        }
        //        for(int i=0;i<vertices2D.size();i++){
        //            tempVertices2D[i].clear();
        //        }
        vertices2D.clear();
        tempVertices2D.clear();
        
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        
        cameraSelect=ofToInt(currentRadio->getActiveName());
        
        currentMesh=ofToInt(currentRadio->getActiveName())-1;
        
        cameras[guiCam].camera.setGlobalOrientation(cameras[cameraSelect].camera.getGlobalOrientation());
        cameras[guiCam].camera.setGlobalPosition(cameras[cameraSelect].camera.getGlobalPosition());
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
        if(translateX2D!=NULL){
            translateX2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].translate.x));
        }
        if(translateY2D!=NULL){
            translateY2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].translate.y));
        }
        if(scale2D!=NULL){
            scale2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].scale));
        }
        if(featherRight2D!=NULL){
            featherRight2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].right));
        }
        if(featherLeft2D!=NULL){
            featherLeft2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].left));
        }
    }
    
    else if(name == "CHANGE MODE"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        setGUIVisible(false);
        
        if(selected=="Camera Position"){
            adjustMode=ADJUST_MODE_CAMERA;
        }
        
        else if(selected=="Camera Orientation"){
            adjustMode=ADJUST_MODE_LOOK_AT;
        }
        
        else if(selected=="Viewport Position"){
            adjustMode=ADJUST_MODE_VIEWPORT;
        }
        
        else if(selected=="3D Meshes"){
            adjustMode=ADJUST_MODE_MESH;
            
        }
        
        else if(selected=="2D Meshes"){
            adjustMode=ADJUST_MODE_2D;
            translateX2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].translate.x));
            translateY2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].translate.y));
            scale2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].scale));
            featherRight2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].right));
            featherLeft2D->setTextString(ofToString(cameras[cameraSelect].meshObjects[currentMesh].left));
        }
        
        else if (selected=="Masks"){
            adjustMode=ADJUST_MODE_MASK;
        }
        setGUIVisible(true);
    }
    
    else if(name=="SELECTION TYPE"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        if(selected=="Pointer"){
            magnetGUI->setVisible(true);
            hideMagnetTypes();
            selectMode=SELECT_MODE_POINTER;
            setEaseHeights(true,false,false,false);
        }
        else{
            magnetGUI->setVisible(true);
            easeTypeRadio->activateToggle("None");
            hideMagnetTypes();
            if(selected=="Paintbrush"){
                selectMode=SELECT_MODE_PAINTBRUSH;
                setEaseHeights(true,false,false,true);
            }
            else if(selected=="Pen"){
                selectMode=SELECT_MODE_PEN;
                setEaseHeights(true,true,false,false);
            }
            else if(selected=="Double Pen"){
                selectMode=SELECT_MODE_DOUBLE_PEN;
                setEaseHeights(false, true, true,false);
            }
        }
    }
    
    else if(name=="Blend Amount"){
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            magnetRadius=ofToFloat(ti->getTextString());
            calculateMagnetPoints();
        }
    }
    
    else if(name=="Brush Radius"){
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER||ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            brushRadius=ofToFloat(ti->getTextString());
        }
    }
    
    else if(name=="CLEAR SELECTION"){
        moveVertices.clear();
        tempVertices.clear();
        magnetVertices.clear();
        penPoly.clear();
        vertices2D.clear();
        tempVertices2D.clear();
    }
    
    else if(name=="RELOAD MESH"){
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
    
    else if(name=="RELOAD SELECTED"){
        resetSelected();
    }
    
    else if(name == "RESET WARP")
    {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        if(button->getValue()==1){
            cameras[cameraSelect].meshObjects[currentMesh].warped=cameras[cameraSelect].meshObjects[currentMesh].originals;
        }
    }
    
    else if(name=="START SELECTION"){
        penPoly.clear();
        bNewPen=true;
        bDrawingPen=true;
        penButton->setColorFill(OFX_UI_COLOR_FILL);
    }
    
    else if(name=="START OUTER"){
        cout<<"double button"<<endl;
        doublePoly.clear();
        bNewDouble=true;
        bDrawingDouble=true;
        doublePenButton->setColorFill(OFX_UI_COLOR_FILL);
    }
    
    else if(name=="EASE TYPE"){
        ofxUIRadio *currentRadio = (ofxUIRadio *) e.widget;
        string selected=currentRadio->getActiveName();
        if(selected=="None"){
            hideMagnetTypes();
            easeMode=EASE_MODE_NONE;
            magnetGUI->setHeight(220);
        }
        else if(selected=="Ease In"){
            hideMagnetTypes();
            easeInLabel->setVisible(true);
            easeInMethod->setVisible(true);
            easeInLabel->getRect()->setY(easeHeight);
            easeInMethod->getRect()->setY(easeHeight+20);
            easeMode=EASE_MODE_IN;
            magnetGUI->setHeight(400);
        }
        else if(selected=="Ease Out"){
            hideMagnetTypes();
            easeOutLabel->setVisible(true);
            easeOutMethod->setVisible(true);
            easeOutLabel->getRect()->setY(easeHeight);
            easeOutMethod->getRect()->setY(easeHeight+20);
            easeMode=EASE_MODE_OUT;
            magnetGUI->setHeight(400);
        }
        else if(selected=="Ease Both"){
            hideMagnetTypes();
            easeBothLabel->setVisible(true);
            easeBothMethod->setVisible(true);
            easeBothLabel->getRect()->setY(easeHeight);
            easeBothMethod->getRect()->setY(easeHeight+20);
            easeMode=EASE_MODE_BOTH;
            magnetGUI->setHeight(400);
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
            easeMode==EASE_MODE_SEPARATE;
            magnetGUI->setHeight(580);
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
    }
    
    else if(name=="START MASK"){
        if(adjustMode==ADJUST_MODE_MASK&&bDrawingMask==false){
            cameras[cameraSelect].addMask();
            bNewMask=true;
            bDrawingMask=true;
            updateMasks();
            maskButton->setColorFill(OFX_UI_COLOR_FILL);
        }
    }
}

void ModelMapper::hideMagnetTypes(){
    easeInLabel->setVisible(false);
    easeInMethod->setVisible(false);
    easeInMethod->activateToggle("Linear");
    easeOutLabel->setVisible(false);
    easeOutMethod->setVisible(false);
    easeOutMethod->activateToggle("Linear");
    easeBothLabel->setVisible(false);
    easeBothMethod->setVisible(false);
    easeBothMethod->activateToggle("Linear");
    brushRadiusLabel->setVisible(false);
    brushRadiusSet->setVisible(false);
}

void ModelMapper::setEaseHeights(bool radiusSpacer, bool pen1, bool pen2, bool brushRadius){
    
    magnetRadiusLabel->setVisible(radiusSpacer);
    magnetRadiusSet->setVisible(radiusSpacer);
    brushRadiusLabel->setVisible(brushRadius);
    brushRadiusSet->setVisible(brushRadius);
    penButton->setVisible(pen1);
    doublePenButton->setVisible(pen2);
    int start;
    
    if(radiusSpacer&&!brushRadius){
        start=80;
        magnetGUI->setHeight(220);
    }
    else if(brushRadius){
        start=130;
        magnetGUI->setHeight(280);
    }
    else{
        start=30;
        magnetGUI->setHeight(220);
    }
    
    if(pen2){
        doublePenButton->getRect()->setY(start+30);
        penButton->getRect()->setY(start);
        easeTypeHeight=start+70;
        magnetRadiusSpacer->getRect()->setY(start+66);
        magnetGUI->setHeight(280);
    }
    else if(pen1){
        easeTypeHeight=start+30;
        penButton->getRect()->setY(start);
        magnetRadiusSpacer->getRect()->setY(start+26);
        magnetGUI->setHeight(280);
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
    adjustModes.push_back("3D Meshes"); adjustModes.push_back("2D Meshes");adjustModes.push_back("Masks");
    
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
    setMaskGUI();
    setMesh2DGUI();
}

float ModelMapper::magnetMap(float distance, float radius){
    float modifier;
    
    if (easeMode==EASE_MODE_NONE){
        modifier=0;
    }
    
    else {
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
            if(distance>radius/2){
                easingType=ofxTween::easeOut;
                currentEase=easeOutStyle;
            }
            else{
                easingType=ofxTween::easeIn;
                currentEase=easeInStyle;
            }
        }
        
        if(currentEase==EASE_STYLE_LINEAR){
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

void ModelMapper::setGUIVisible(bool hide){
    if(hide==false){
        mainGUI->setVisible(false);
        positionGUI->setVisible(false);
        orientationGUI->setVisible(false);
        viewportGUI->setVisible(false);
        meshGUI->setVisible(false);
        mesh2DGUI->setVisible(false);
        magnetGUI->setVisible(false);
        maskGUI->setVisible(false);
    }
    else{
        mainGUI->setVisible(true);
        if(adjustMode==ADJUST_MODE_CAMERA) positionGUI->setVisible(true);
        else if(adjustMode==ADJUST_MODE_LOOK_AT) orientationGUI->setVisible(true);
        else if(adjustMode==ADJUST_MODE_VIEWPORT) viewportGUI->setVisible(true);
        else if(adjustMode==ADJUST_MODE_MESH) {
            meshGUI->setVisible(true);
            magnetGUI->setVisible(true);
        }
        else if(adjustMode==ADJUST_MODE_MASK)maskGUI->setVisible(true);
        else if(adjustMode==ADJUST_MODE_2D) mesh2DGUI->setVisible(true);
    }
}

void ModelMapper::resetSelected(){
    ofxAssimpModelLoader reload;
    if(meshType==MESH_DETAIL){
        reload.loadModel(detailMesh);
    }
    else if(meshType==MESH_MASS){
        reload.loadModel(massMesh);
    }
    if(moveVertices.size()>0){
        for(int j=0;j<numMeshes;j++){
            for(int k=0;k<moveVertices[j].size();k++){
                cameras[cameraSelect].mesh[j].setVertex(moveVertices[j][k].index,reload.getMesh(whichMeshes[j]).getVertex(moveVertices[j][k].index ));
            }
        }
    }
    cout<<"Reloaded Selected"<<endl;
}

void ModelMapper::set2D(int _meshNum){
    
    for(int c=0;c<numCams;c++){
        
        ofPoint translate = ofPoint(settings["cameras"][c]["meshes"][_meshNum]["translate"]["x"].asFloat(), settings["cameras"][c]["meshes"][_meshNum]["translate"]["y"].asFloat());
        float scale = settings["cameras"][c]["meshes"][_meshNum]["scale"].asFloat();
        int horizGrid = settings["cameras"][c]["meshes"][_meshNum]["grid"]["x"].asInt();
        int vertGrid = settings["cameras"][c]["meshes"][_meshNum]["grid"]["y"].asInt();
        ofPoint texPos = ofPoint(settings["cameras"][c]["meshes"][_meshNum]["texPos"]["x"].asFloat(), settings["cameras"][c]["meshes"][_meshNum]["texPos"]["y"].asFloat());
        int texWidth = settings["cameras"][c]["meshes"][_meshNum]["texWidth"].asInt();
        int texHeight = settings["cameras"][c]["meshes"][_meshNum]["texHeight"].asInt();
        vector< vector<ofPoint> > originals;
        vector< vector<ofPoint> > warped;
        int count = 0;
        float featherRight=settings["cameras"][c]["meshes"][_meshNum]["feather"]["right"].asFloat();
        float featherLeft=settings["cameras"][c]["meshes"][_meshNum]["feather"]["left"].asFloat();
        
        for(int i=0; i<vertGrid-1; i++){
            for(int j=0; j<horizGrid-1; j++){
                if(count<(horizGrid-1)*(vertGrid-1)){
                    vector<ofPoint> loadOrig, loadWarped;
                    ofPoint tempPoint;
                    ofPoint warpedPos;
                    tempPoint.x=j*(texWidth/(horizGrid-1));
                    tempPoint.y=i*(texHeight/(vertGrid-1));
                    loadOrig.push_back(tempPoint);
                    
                    
                    if(settings["cameras"][c]["meshes"][_meshNum]["warped"].size()==((vertGrid-1)*(horizGrid-1))){
                        warpedPos=ofPoint(settings["cameras"][c]["meshes"][_meshNum]["warped"][count][0]["x"].asFloat(), settings["cameras"][c]["meshes"][_meshNum]["warped"][count][0]["y"].asFloat());
                        loadWarped.push_back(warpedPos);
                    }
                    
                    else{
                        loadWarped.push_back(tempPoint);
                    }
                    
                    tempPoint.x=(j+1)*(texWidth/(horizGrid-1));
                    tempPoint.y=i*(texHeight/(vertGrid-1));
                    loadOrig.push_back(tempPoint);
                    
                    if(settings["cameras"][c]["meshes"][_meshNum]["warped"].size()==((vertGrid-1)*(horizGrid-1))){
                        warpedPos=ofPoint(settings["cameras"][c]["meshes"][_meshNum]["warped"][count][1]["x"].asFloat(), settings["cameras"][c]["meshes"][_meshNum]["warped"][count][1]["y"].asFloat());
                        loadWarped.push_back(warpedPos);
                    }
                    
                    else{
                        loadWarped.push_back(tempPoint);
                    }
                    
                    tempPoint.x=(j+1)*(texWidth/(horizGrid-1));
                    tempPoint.y=(i+1)*(texHeight/(vertGrid-1));
                    loadOrig.push_back(tempPoint);
                    
                    if(settings["cameras"][c]["meshes"][_meshNum]["warped"].size()==((vertGrid-1)*(horizGrid-1))){
                        warpedPos=ofPoint(settings["cameras"][c]["meshes"][_meshNum]["warped"][count][2]["x"].asFloat(), settings["cameras"][c]["meshes"][_meshNum]["warped"][count][2]["y"].asFloat());
                        loadWarped.push_back(warpedPos);
                    }
                    
                    else{
                        loadWarped.push_back(tempPoint);
                    }
                    
                    tempPoint.x=j*(texWidth/(horizGrid-1));
                    tempPoint.y=(i+1)*(texHeight/(vertGrid-1));
                    loadOrig.push_back(tempPoint);
                    
                    if(settings["cameras"][c]["meshes"][_meshNum]["warped"].size()==((vertGrid-1)*(horizGrid-1))){
                        warpedPos=ofPoint(settings["cameras"][c]["meshes"][_meshNum]["warped"][count][3]["x"].asFloat(), settings["cameras"][c]["meshes"][_meshNum]["warped"][count][3]["y"].asFloat());
                        loadWarped.push_back(warpedPos);
                    }
                    
                    else{
                        loadWarped.push_back(tempPoint);
                    }
                    
                    originals.push_back(loadOrig);
                    
                    warped.push_back(loadWarped);
                    loadOrig.clear();
                    loadWarped.clear();
                    count++;
                    
                    
                }
            }
        }
        cameras[c].set2D(_meshNum, translate, scale, horizGrid, vertGrid, texPos, texWidth, texHeight, featherRight, featherLeft, originals, warped);
        originals.clear();
        warped.clear();
    }
    
}

void ModelMapper::setMeshDraw(int _cam, vector<int> which){
    cameras[_cam].which=which;
}

void ModelMapper::fadeIn(int type){
    bTransitioning=true;
    bTransitionStarted=true;
    bTransitionFinished=false;
    transitionTimer=ofGetElapsedTimeMillis();
    if (type==TRANSITION_AMBIENT_GRADIENT){
        fadeFrame=&ambientGradientFrame;
    }
    else if(type==TRANSITION_ARTS){
        fadeFrame=&artsFrame;
    }
    else if(type==TRANSITION_GASTRONOMY){
        fadeFrame=&gastronomyFrame;
    }
    else if(type==TRANSITION_LEISURE){
        fadeFrame=&leisureFrame;
    }
    else if(type==TRANSITION_SHOPPING){
        fadeFrame=&shoppingFrame;
    }
    else if(type==TRANSITION_MARKETS){
        fadeFrame=&marketsFrame;
    }
    else if(type==TRANSITION_END){
        fadeFrame=&endFrame;
    }
}