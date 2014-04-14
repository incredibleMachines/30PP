#include "testApp.h"
#include "ofxCocoa.h"
#include "ModelMapper/ModelMapper.h"

//--------------------------------------------------------------
void testApp::setup() {
	printf("Window dimensions: %i %i\n", ofGetWidth(), ofGetHeight());
	
	ofBackground(0, 0, 0);
    
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    
	ofSetFrameRate(120);			// run as fast as you can
    
    bInited=false;
    
    meshTexture.push_back(new ofTexture());
    
    ofTextureData data;
    data.textureTarget=GL_TEXTURE_RECTANGLE;
    cout<<data.textureTarget<<endl;
	data.width = TEX_WIDTH;
	data.height = TEX_HEIGHT;
	data.tex_w = TEX_WIDTH;
	data.tex_h = TEX_HEIGHT;
	data.tex_t = TEX_WIDTH;
	data.tex_u = TEX_HEIGHT;
    
//    meshTexture[0]->allocate(TEX_WIDTH, TEX_HEIGHT, GL_TEXTURE_2D, GL_TEXTURE_2D, GL_RGB);

    meshTexture[0]->allocate(data);
//    test.loadImage("Sequence 01.jpg");
//    meshTexture[0]=&test.getTextureReference();
//    
    MSA::ofxCocoa::initPlayer("output.mov", meshTexture[0]->texData.textureID);
    
    //----------MODEL MAPPER SETUP
    
    //Load mesh vector to select which meshes within obj to use
    vector<int> _meshesLoad;
    _meshesLoad.push_back(0);
//    _meshesLoad.push_back(1);
//    _meshesLoad.push_back(2);
    numMesh=_meshesLoad.size();
    
    //setup ModelMapper - setup(number of Cameras, which camera is the gui, vector of mesh ids to draw)
    map.setup(4,0,_meshesLoad);
    
    //set path to obj file to use in setup
    map.setMassMesh("04_04.obj");
    
    //----------SOCKET HANDLER SETUP
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    if(bInited==false&&socketHandler.eventHandler.eventsInited==true){
        bInited=true;
//        MSA::ofxCocoa::initPlayer(socketHandler.eventHandler.movieFile, meshTexture[0]->texData.textureID);
//        MSA::ofxCocoa::pausePlayer();
//        MSA::ofxCocoa::setTime(time);
//        MSA::ofxCocoa::startPlayer();
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
    
//    ofTextureData data=meshTexture[0]->getTextureData();
//    data.textureTarget=GL_TEXTURE_2D;
//    cout<<data.textureTarget<<endl;
    
    //Get Texture data from CVOpenGLTexture in ofxCocoa
    meshTexture[0]->setUseExternalTextureID(MSA::ofxCocoa::getTextureID());
    ofTextureData data=meshTexture[0]->getTextureData();
    data.textureTarget=GL_TEXTURE_RECTANGLE;
    
    //Make sure texture data has correct settings for display
//    data.tex_t = (float)(data.width) / (float)data.tex_w;
//    data.tex_u = (float)(data.height) / (float)data.tex_h;
    
    //Pass texture into ModelMapper
    map.update(meshTexture);
    
    
    //------ UPDATE DEM SOCKETS
    socketHandler.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    map.draw();
    
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
    }
    
}

void testApp::exit(){
    
}
