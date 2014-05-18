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
//    MSA::ofxCocoa::initPlayer("../../../ControllerApp/includes/videos/concatOutput_3D.mov", meshTexture->texData.textureID);
    MSA::ofxCocoa::initPlayer("calibration.mov", meshTexture->texData.textureID);
    //----------MODEL MAPPER   SETUP
    
    //Load mesh vector to select which meshes within obj to use
    vector<vector<int> > _meshesLoad;
    vector<int> tempMesh;
    tempMesh.push_back(0);
    tempMesh.push_back(MESH_2D);
    _meshesLoad.push_back(tempMesh);
    tempMesh.clear();
    tempMesh.push_back(1);
    tempMesh.push_back(MESH_2D);
    _meshesLoad.push_back(tempMesh);
    tempMesh.clear();
    tempMesh.push_back(2);
    tempMesh.push_back(MESH_2D);
    _meshesLoad.push_back(tempMesh);
    tempMesh.clear();
    tempMesh.push_back(3);
    tempMesh.push_back(MESH_3D);
    tempMesh.push_back(0);
    _meshesLoad.push_back(tempMesh);
    tempMesh.clear();
    
    numMesh=_meshesLoad.size();
    
    //setup ModelMapper - setup(number of Cameras, which camera is the gui, vector of mesh ids to draw)
    map.setup(4,0,_meshesLoad);
    
    //set path to obj file to use in setup
    map.setMassMesh("UV Sculpt_LP 01.obj");
    
    //----------SOCKET HANDLER SETUP
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
    MSA::ofxCocoa::hideCursor();
    bCheckingTime=false;
    loopMode=DEFAULT_LOOP;
    
    numScreens=1;
    startOffset=3.0;
    bScreenRestart=false;
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    //------ UPDATE DEM SOCKETS
    socketHandler.update();
    
    if(bInited==false&&socketHandler.eventHandler.eventsInited==true){
        bInited=true;
        bCheckingTime=true;
        initVariables();
    }
    
    if(socketHandler.eventHandler.bTriggerEvent==true){
        
        socketHandler.eventHandler.bTriggerEvent=false;
        loadTime=socketHandler.eventHandler.currentStart;
        MSA::ofxCocoa::startPlayer();
        bCheckingTime=false;
        
        for(int i=0; i<socketHandler.eventHandler.events.size();i++){
            if(socketHandler.eventHandler.currentEvent==socketHandler.eventHandler.events[i].title){
                currentEnd=socketHandler.eventHandler.events[i].startTime+socketHandler.eventHandler.events[i].duration;
            }
        }
        
        if(socketHandler.eventHandler.currentEvent=="default"){
            map.fadeIn(TRANSITION_DEFAULT);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="end"){
            if(loopMode==AMBIENT_LOOP){
                loadTime=socketHandler.eventHandler.events[0].startTime;
                map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
            }
            else if (loopMode==DEFAULT_LOOP){
                loadTime=socketHandler.eventHandler.events[1].startTime;
                map.fadeIn(TRANSITION_GASTRONOMY);
            }
        }
        
        else if(socketHandler.eventHandler.currentEvent=="ambient_gradient"){
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="gastronomy"){
            loadTime+=startOffset;
            map.fadeIn(TRANSITION_GASTRONOMY);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="markets"){
            loadTime+=startOffset;
            map.fadeIn(TRANSITION_MARKETS);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="shopping"){
            loadTime+=startOffset;
            map.fadeIn(TRANSITION_SHOPPING);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="art-design"){
            map.fadeIn(TRANSITION_ARTS);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="leisure"){
            loadTime+=startOffset;
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
            loadTime=socketHandler.eventHandler.events[0].startTime;
            map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
        }
        else if (loopMode==DEFAULT_LOOP){
            loadTime=socketHandler.eventHandler.events[1].startTime;
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
    
 
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if(MSA::ofxCocoa::getScreens()>=numScreens&&bScreenRestart==false){
        map.draw();
    }
    
    else if(MSA::ofxCocoa::getScreens()>=numScreens&&bScreenRestart==true){
        bStartScreenRestart=true;
    }
    
    else {
        bScreenRestart=true;
    }

    
    if(bScreenRestart==true&&bStartScreenRestart==true){
        socketHandler.sendSocketCmd(RESTART_REQ);
        bStartScreenRestart=false;
        bScreenRestart=false;
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

        case '{':
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()-10);
            break;
        case '}':
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()+10);
            break;
        case '[':
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()-10);
            break;            
        case ']':
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()+10);
            break;
        case 'Q':
            socketHandler.sendSocketCmd(CLOSE_REQ);
            break;
            
    }
    
}

void testApp::exit(){
    
}

void testApp::initVariables(){
    
    if(loopMode==DEFAULT_LOOP){
        currentEnd=socketHandler.eventHandler.events[1].startTime+socketHandler.eventHandler.events[1].duration;
        currentTransition=TRANSITION_GASTRONOMY;
        loadTime=socketHandler.eventHandler.events[1].startTime;
        map.fadeIn(TRANSITION_GASTRONOMY);
    }
    else if(loopMode==AMBIENT_LOOP){
        currentEnd=socketHandler.eventHandler.events[0].startTime+socketHandler.eventHandler.events[0].duration;
        currentTransition=TRANSITION_AMBIENT_GRADIENT;
        loadTime=socketHandler.eventHandler.events[0].startTime;
        map.fadeIn(TRANSITION_AMBIENT_GRADIENT);
    }
    
    //Gastronomy Default
    pauseTimes.push_back(663);
    pauseTimes.push_back(669.17);
    pauseTimes.push_back(680.43);
    pauseTimes.push_back(684.22);
    //Markets Default
    pauseTimes.push_back(710.38);
    pauseTimes.push_back(717);
    pauseTimes.push_back(728);
    pauseTimes.push_back(732);
    //Shopping Default
    pauseTimes.push_back(758);
    pauseTimes.push_back(764.06);
    pauseTimes.push_back(774.32);
    pauseTimes.push_back(780.16);
    //Art & Design Default
    pauseTimes.push_back(804.17);
    pauseTimes.push_back(811.03);
    pauseTimes.push_back(821.25);
    pauseTimes.push_back(826.12);
    //Leisure Default
    pauseTimes.push_back(852.11);
    pauseTimes.push_back(858.09);
    pauseTimes.push_back(868.36);
    pauseTimes.push_back(874.14);
    
    //Gastronomy Detail
    pauseTimes.push_back(908.46);
    pauseTimes.push_back(915.09);
    pauseTimes.push_back(926.11);
    pauseTimes.push_back(930.17);
    pauseTimes.push_back(942.35);
    pauseTimes.push_back(947.14);
    pauseTimes.push_back(960.09);
    
    //Mrkets Detail
    pauseTimes.push_back(1005.11);
    pauseTimes.push_back(1012.07);
    pauseTimes.push_back(1022.03);
    pauseTimes.push_back(1027.07);
    
    //Shopping Detail
    pauseTimes.push_back(1077.47);
    pauseTimes.push_back(1084.04);
    pauseTimes.push_back(1095.09);
    pauseTimes.push_back(1100.22);
    pauseTimes.push_back(1112.04);
    pauseTimes.push_back(1116.08);
    pauseTimes.push_back(1128.35);
    pauseTimes.push_back(1133.39);
    
    //Arts & Design Detail
    pauseTimes.push_back(1183.27);
    pauseTimes.push_back(1190.01);
    pauseTimes.push_back(1200.19);
    pauseTimes.push_back(1205.11);
    pauseTimes.push_back(1215.24);
    pauseTimes.push_back(1222.20);
    pauseTimes.push_back(1235.06);
    
    //Leisure Detail
    pauseTimes.push_back(1281.00);
    pauseTimes.push_back(1287.06);
    pauseTimes.push_back(1297.43);
    pauseTimes.push_back(1303.12);
    pauseTimes.push_back(1314.26);
    pauseTimes.push_back(1319.26);
}
