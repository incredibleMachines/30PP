#include "testApp.h"
#include "ofxCocoa.h"
#include "ModelMapper/ModelMapper.h"

//--------------------------------------------------------------
void testApp::setup() {
	printf("Window dimensions: %i %i\n", ofGetWidth(), ofGetHeight());
	
	ofBackground(0, 0, 0);
    
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
	ofSetFrameRate(120);			// run as fast as you can
    
    meshTexture.push_back(new ofTexture());
    meshTexture[0]->allocate(TEX_WIDTH, TEX_HEIGHT, GL_RGB);
    MSA::ofxCocoa::initPlayer("long_2048.mov", meshTexture[0]->texData.textureID);
    
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
    map.setMassMesh("mesh/mesh.obj");
    
    //----------SOCKET HANDLER SETUP
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    //Get Texture data from CVOpenGLTexture in ofxCocoa
    meshTexture[0]->setUseExternalTextureID(MSA::ofxCocoa::getTextureID());
    
    //Make sure texture data has correct settings for display
	ofTextureData& data = meshTexture[0]->getTextureData();
	data.textureTarget = GL_TEXTURE_RECTANGLE;
	data.width = TEX_WIDTH;
	data.height = TEX_HEIGHT;
	data.tex_w = TEX_WIDTH;
	data.tex_h = TEX_HEIGHT;
	data.tex_t = TEX_WIDTH;
	data.tex_u = TEX_HEIGHT;
    
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
