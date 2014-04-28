#include "testApp.h"
#include "ofxCocoa.h"
#include "ModelMapper/ModelMapper.h"

//--------------------------------------------------------------
void testApp::setup() {
	printf("Window dimensions: %i %i\n", ofGetWidth(), ofGetHeight());
	
    ofSetEscapeQuitsApp(false);
    
	ofBackground(0,0,0);
    
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    
	ofSetFrameRate(120);			// run as fast as you can
    
    bInited=false;
    
    ofTextureData data;
    data.textureTarget=GL_TEXTURE_RECTANGLE;
    cout<<data.textureTarget<<endl;
	data.width = TEX_WIDTH;
	data.height = TEX_HEIGHT;
	data.tex_w = TEX_WIDTH;
	data.tex_h = TEX_HEIGHT;
	data.tex_t = TEX_WIDTH;
	data.tex_u = TEX_HEIGHT;

    meshTexture = new ofTexture();
    meshTexture->allocate(data);

    MSA::ofxCocoa::initPlayer("/Users/IM_Laptop_01/Downloads/081/apps/30PP/ControllerApp/includes/videos/concatOutput.mov", meshTexture->texData.textureID);

//    MSA::ofxCocoa::initPlayer("../../../ControllerApp/includes/videos/concatOutput.mov", meshTexture->texData.textureID);
//    MSA::ofxCocoa::initPlayer("checker.mov", meshTexture->texData.textureID);
    //----------MODEL MAPPER SETUP
    
    //Load mesh vector to select which meshes within obj to use
    vector<int> _meshesLoad;
    _meshesLoad.push_back(0);
    _meshesLoad.push_back(1);
    _meshesLoad.push_back(2);
    numMesh=_meshesLoad.size();
    
    //setup ModelMapper - setup(number of Cameras, which camera is the gui, vector of mesh ids to draw)
    map.setup(4,0,_meshesLoad);
    
    //set path to obj file to use in setup
    map.setMassMesh("UV walls_05.obj");
    map.set2D(0);
    map.set2D(1);
    map.set2D(2);
    
    vector<int> which;
    which.push_back(0);
    map.setMeshDraw(1,which);
    which.clear();
    which.push_back(2);
    map.setMeshDraw(2,which);
    which.clear();
    which.push_back(1);
    map.setMeshDraw(3,which);
    
    //----------SOCKET HANDLER SETUP
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
    MSA::ofxCocoa::hideCursor();
    bCheckingTime=false;
    loopMode=DEFAULT_LOOP;
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    if(bInited==false&&socketHandler.eventHandler.eventsInited==true){
        bInited=true;
        bCheckingTime=true;
        initVariables();
        if(loopMode==DEFAULT_LOOP){
            MSA::ofxCocoa::setTime(650.0);
        }
    }
    
    if(socketHandler.eventHandler.bTriggerEvent==true){
        
        
//        if(map.adjustMode==ADJUST_MODE_LOCKED){
//            MSA::ofxCocoa::hideCursor();
//        }
        
        socketHandler.eventHandler.bTriggerEvent=false;
        loadTime=socketHandler.eventHandler.currentStart;
        MSA::ofxCocoa::startPlayer();
        
        for(int i=0; i<socketHandler.eventHandler.events.size();i++){
            if(socketHandler.eventHandler.currentEvent==socketHandler.eventHandler.events[i].title){
                currentEnd=socketHandler.eventHandler.events[i].startTime+socketHandler.eventHandler.events[i].duration;
            }
        }
        
        if(socketHandler.eventHandler.currentEvent=="default"){
            map.fadeIn(TRANSITION_GASTRONOMY);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="end"){
            loadTime=0.0;
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="ambient_gradient"){
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="gastronomy"){
            map.fadeIn(TRANSITION_GASTRONOMY);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="markets"){
            map.fadeIn(TRANSITION_MARKETS);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="shopping"){
            map.fadeIn(TRANSITION_SHOPPING);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="art-design"){
            map.fadeIn(TRANSITION_ARTS);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="leisure"){
            map.fadeIn(TRANSITION_LEISURE);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="pause"){
            bool bCanPause=false;
            for(int i=0; i<pauseTimes.size();i++){
                if(pauseTimes[i]>MSA::ofxCocoa::getCurrentTime()){
                    bCanPause=true;
                    pauseTime=pauseTimes[i];
                    break;
                }
            }
            if(bCanPause==true){
                bPausing=true;
            }
        }
        
        else if(socketHandler.eventHandler.currentEvent=="resume"){
            MSA::ofxCocoa::startPlayer();
        }
        
    }
    
    if(bCheckingTime==true&&MSA::ofxCocoa::getCurrentTime()>currentEnd-1){
        if(loopMode==AMBIENT_LOOP){
            loadTime=0.0;
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
        }
        else if (loopMode==DEFAULT_LOOP){
            loadTime=650.0;
            map.fadeIn(TRANSITION_GASTRONOMY);
        }
        bCheckingTime=false;
    }
    
    if(map.bTransitioning==true&&map.bTransitionLoading==false&&map.bTransitionStarted==false&&map.bTransitionFinished==false){
        MSA::ofxCocoa::setTime(loadTime);
        MSA::ofxCocoa::startPlayer();
        map.bTransitionLoading=true;
        map.transitionTimer=ofGetElapsedTimeMillis();
        bCheckingTime=true;
    }
    
    if(map.bLocked==true){
        MSA::ofxCocoa::hideCursor();
        map.bLocked=false;
    }
    
    else if(map.bUnlocked==true){
        MSA::ofxCocoa::showCursor();
        map.bUnlocked=false;
    }
    
    if(bPausing==true&&MSA::ofxCocoa::getCurrentTime()>pauseTime){
        bPausing=false;
        MSA::ofxCocoa::pausePlayer();
    }
    
    //Get Texture data from CVOpenGLTexture in ofxCocoa
    meshTexture->setUseExternalTextureID(MSA::ofxCocoa::getTextureID());
    ofTextureData data=meshTexture->getTextureData();
    data.textureTarget=GL_TEXTURE_RECTANGLE;
    
    //Pass texture into ModelMapper
    map.update(meshTexture);
    
    //------ UPDATE DEM SOCKETS
    socketHandler.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if(MSA::ofxCocoa::getScreens()>0){
        map.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case 'p':
            MSA::ofxCocoa::pausePlayer();
            break;
        case 'P':
            MSA::ofxCocoa::startPlayer();
            break;

        case '[':
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()-10);
            break;            
        case ']':
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()+10);
            break;
            
    }
    
}

void testApp::exit(){
    
}

void testApp::initVariables(){
    currentEnd=650;
    currentTransition=TRANSITION_AMBIENT_GRADIENT;
    
    float tempPause;
    tempPause=socketHandler.eventHandler.events[1].startTime+23;
    pauseTimes.push_back(tempPause);
    tempPause+=5.7;
    pauseTimes.push_back(tempPause);
    tempPause+=10.8;
    pauseTimes.push_back(tempPause);
    tempPause+=5.2;
    pauseTimes.push_back(tempPause);
    tempPause+=26;
    pauseTimes.push_back(tempPause);
    tempPause+=5.7;
    pauseTimes.push_back(tempPause);
    tempPause+=10.8;
    pauseTimes.push_back(tempPause);
    tempPause+=5.2;
    pauseTimes.push_back(tempPause);
}
