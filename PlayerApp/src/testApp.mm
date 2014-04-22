#include "testApp.h"
#include "ofxCocoa.h"
#include "ModelMapper/ModelMapper.h"

//--------------------------------------------------------------
void testApp::setup() {
	printf("Window dimensions: %i %i\n", ofGetWidth(), ofGetHeight());
	
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
    
    MSA::ofxCocoa::initPlayer("default.mov", meshTexture->texData.textureID);
    
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
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    if(bInited==false&&socketHandler.eventHandler.eventsInited==true){
        bInited=true;
    }
    
    if(socketHandler.eventHandler.bTriggerEvent==true){
        
        socketHandler.eventHandler.bTriggerEvent=false;
        
        if(socketHandler.eventHandler.bPlayEvent==true){
            MSA::ofxCocoa::setTime(socketHandler.eventHandler.currentStart);
        }
        
        if(socketHandler.eventHandler.bPlaying==true){
            MSA::ofxCocoa::startPlayer();
        }
        else{
            MSA::ofxCocoa::pausePlayer();
        }
        
    }
    
    if(map.bTransitioning==true&&map.bTransitionLoading==false&&map.bTransitionStarted==false&&map.bTransitionFinished==false){
        MSA::ofxCocoa::setTime(loadTime);
        map.bTransitionLoading=true;
        map.transitionTimer=ofGetElapsedTimeMillis();
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
    else{
        ofDrawBitmapString("OOOOPS", 100,100);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case ' ':
            if(map.adjustMode!=ADJUST_MODE_LOCKED){
                MSA::ofxCocoa::hideCursor();
            }
            else if(map.bShiftPressed==true){
                MSA::ofxCocoa::showCursor();
            }
            break;
        case 'p':
            MSA::ofxCocoa::pausePlayer();
            break;
        case 'P':
            MSA::ofxCocoa::startPlayer();
            break;
        case '1':
            map.fadeIn(TRANSITION_MARKETS);
            loadTime=44;
            break;
        case '2':
            map.fadeIn(TRANSITION_ARTS);
            loadTime=79;
            break;
        case '3':
            map.fadeIn(TRANSITION_SHOPPING);
            loadTime=114;
            break;
        case '4':
            map.fadeIn(TRANSITION_LEISURE);
            loadTime=149;
            break;
        case '5':
            map.fadeIn(TRANSITION_END);
            loadTime=184;
            break;
    }
    
}

void testApp::exit(){
    
}
