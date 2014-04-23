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
    
    MSA::ofxCocoa::initPlayer("../../../ControllerApp/includes/videos/concatOutput.mov", meshTexture->texData.textureID);
    
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
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    if(bInited==false&&socketHandler.eventHandler.eventsInited==true){
        bInited=true;
        initVariables();
    }
    
    if(socketHandler.eventHandler.bTriggerEvent==true){
        
        socketHandler.eventHandler.bTriggerEvent=false;
        
        if(socketHandler.eventHandler.currentEvent=="default"){
            map.fadeIn(TRANSITION_GASTRONOMY);
            loadTime=650;
            cout<<"play"<<endl;
        }
        else if(socketHandler.eventHandler.currentEvent=="gastronomy"){
            map.fadeIn(TRANSITION_GASTRONOMY);
            loadTime=650;
        }
        
        else if(socketHandler.eventHandler.currentEvent=="end"){
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
            loadTime=0;
        }
        
        else if(socketHandler.eventHandler.currentEvent=="ambient"){
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
            loadTime=0;
        }
        
        else if(socketHandler.eventHandler.currentEvent=="art-design"){
            map.fadeIn(TRANSITION_ARTS);
            loadTime=729;
        }
        
        else if(socketHandler.eventHandler.currentEvent=="pause"){
            MSA::ofxCocoa::pausePlayer();
        }
        
        else if(socketHandler.eventHandler.currentEvent=="resume"){
            MSA::ofxCocoa::startPlayer();
        }
        
    }
    
    if(map.bTransitioning==true&&map.bTransitionLoading==false&&map.bTransitionStarted==false&&map.bTransitionFinished==false){
        MSA::ofxCocoa::setTime(loadTime);
        MSA::ofxCocoa::startPlayer();
        map.bTransitionLoading=true;
        map.transitionTimer=ofGetElapsedTimeMillis();
    }
    
    if(map.bLocked==true){
        MSA::ofxCocoa::hideCursor();
        map.bLocked=false;
    }
    
    else if(map.bUnlocked==true){
        MSA::ofxCocoa::showCursor();
        map.bUnlocked=false;
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
        case '1':
            map.fadeIn(TRANSITION_GASTRONOMY);
            loadTime=650;
            break;
        case '2':
            map.fadeIn(TRANSITION_MARKETS);
            loadTime=694;
            break;
        case '3':
            map.fadeIn(TRANSITION_ARTS);
            loadTime=729;
            break;
        case '4':
            map.fadeIn(TRANSITION_SHOPPING);
            loadTime=764;
            break;
        case '5':
            map.fadeIn(TRANSITION_LEISURE);
            loadTime=799;
            break;
        case '6':
            map.fadeIn(TRANSITION_END);
            loadTime=834;
            break;
        case '0':
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
            loadTime=0;
            break;
        case '[':
            socketHandler.sendSocketCmd(INIT_REQ);
            initVariables();
            initCount++;
            break;
            
    }
    
}

void testApp::exit(){
    
}

void testApp::initVariables(){
    
}
