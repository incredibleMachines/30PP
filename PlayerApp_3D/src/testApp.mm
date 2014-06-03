#include "testApp.h"
#include "ofxCocoa.h"
#include "ModelMapper/ModelMapper.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    //User Variable
    //check for init event from sockets
    bInited=false;
    //hide cursor at launch for show mode
    MSA::ofxCocoa::hideCursor();
    //not currently playing and checking for loop or pause point
    bCheckingTime=false;
    //set loop mode to loop either ambient or default content
    loopMode=CLOUDS_LOOP;
    //set number of GL cameras/projectors
    numScreens=3;
    //set how many seconds into detail section to start after transition, determines speed of transition (smaller = longer)
    startOffset=0;
    //set whether to check number of screens b/c less than 3 - will get set to true if cocoa ever detects less than three
    bScreenRestart=false;
	
    //INIT GL CONTEXT VARIABLES
    ofSetEscapeQuitsApp(false);
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
	ofSetFrameRate(120);			// run as fast as you can for display link
    ofBackground(0,0,0);
    
    //VIDEO TEXTURE FOR OFXCOCOA AVPLAYER ITEM VIDEO
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
    
    //LOAD VIDEO
    MSA::ofxCocoa::initPlayer("../../../ControllerApp/includes/videos/concatOutput.mov", meshTexture->texData.textureID);

//     MSA::ofxCocoa::initPlayer("default_leisure.mov", meshTexture->texData.textureID);
 
    //----------MODEL MAPPER   SETUP
    //Load mesh vector to tell ModelMapper which meshes to use 3D obj file and which to set as 2D textures
    
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
    map.setup(4,0,_meshesLoad, TEX_WIDTH, TEX_HEIGHT);
    
    //set path to obj file to use in setup
    map.setMassMesh("UV Sculpt_LP 01.obj");
    
    //----------SOCKET HANDLER SETUP
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    if(ofGetWindowPositionX()!=1920){
        ofSetWindowPosition(-1920,0);
    }
    
    //------ UPDATE DEM SOCKETS
    socketHandler.update();
    
    //INIT VARIABLES IF FIRST TIME INIT COMMAND
    if(bInited==false&&socketHandler.eventHandler.eventsInited==true){
        bInited=true;
        bCheckingTime=true;
        initVariables();
    }
    
    //SOCKET EVENT RECEIVED
    if(socketHandler.eventHandler.bTriggerEvent==true){
        ofSetWindowPosition(-1920,0);
        socketHandler.eventHandler.bTriggerEvent=false;
        
        //set load point to start time received from socket
        loadTime=socketHandler.eventHandler.currentStart;
        
        //play video just in case it's pause
        MSA::ofxCocoa::startPlayer();
        
        //don't check for video end or else will loop on frame one and cause an extra frame
        bCheckingTime=false;
        
        //load end time for current event
        for(int i=0; i<socketHandler.eventHandler.events.size();i++){
            if(socketHandler.eventHandler.currentEvent==socketHandler.eventHandler.events[i].title){
                currentEnd=socketHandler.eventHandler.events[i].startTime+socketHandler.eventHandler.events[i].duration;
            }
        }
        
        if(socketHandler.eventHandler.currentEvent=="default"){
            currentEnd-=5;
            loadTime+=8.5;
            loopMode==CLOUDS_LOOP;
            loadTime+=3;
            map.fadeIn(TRANSITION_DEFAULT);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="end"){
            if (loopMode==DEFAULT_LOOP){
                loadTime=socketHandler.eventHandler.events[5].startTime;
                currentEnd=socketHandler.eventHandler.events[5].startTime+socketHandler.eventHandler.events[5].duration;
                map.fadeIn(TRANSITION_DEFAULT);
            }

            else {
                loadTime=socketHandler.eventHandler.events[2].startTime;
                currentEnd=socketHandler.eventHandler.events[2].startTime+socketHandler.eventHandler.events[2].duration;
                map.fadeIn(TRANSITION_CLOUDS);
            }
        }
        
        else if(socketHandler.eventHandler.currentEvent=="ambient"){
            loadTime=socketHandler.eventHandler.events[2].startTime;
            loopMode==CLOUDS_LOOP;
            map.fadeIn(TRANSITION_CLOUDS);
            
        }
        
        else if(socketHandler.eventHandler.currentEvent=="ambient_gradient"){
            loopMode==GRADIENT_LOOP;
            map.fadeIn(TRANSITION_GRADIENT);

        }
        
        
        else if(socketHandler.eventHandler.currentEvent=="ambient_clouds"){
            loopMode==CLOUDS_LOOP;
            map.fadeIn(TRANSITION_CLOUDS);
            
        }
        
        else if(socketHandler.eventHandler.currentEvent=="ambient_party"){
            loopMode=PARTY_LOOP;
            map.fadeIn(TRANSITION_PARTY);
            
        }
        
        else if(socketHandler.eventHandler.currentEvent=="ambient_waves"){
            loopMode==WAVES_LOOP;
            map.fadeIn(TRANSITION_WAVES);
            
        }
        
        else if(socketHandler.eventHandler.currentEvent=="gastronomy"){
            loopMode==CLOUDS_LOOP;
            loadTime+=startOffset;
            map.fadeIn(TRANSITION_GASTRONOMY);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="markets"){
            loopMode==CLOUDS_LOOP;
            loadTime+=startOffset;
            map.fadeIn(TRANSITION_MARKETS);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="shopping"){
                        loopMode==CLOUDS_LOOP;
            loadTime+=startOffset;
            map.fadeIn(TRANSITION_SHOPPING);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="art-design"){
                        loopMode==CLOUDS_LOOP;
            map.fadeIn(TRANSITION_ARTS);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="leisure"){
                        loopMode==CLOUDS_LOOP;
            loadTime+=startOffset;
            map.fadeIn(TRANSITION_LEISURE);
        }
        
        else if(socketHandler.eventHandler.currentEvent=="pause"){
            bool bCanPause=false;
            
            //look for next pause time
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
    
    
    //check for end of current event and then go to ambient/default start
    if(bCheckingTime==true&&MSA::ofxCocoa::getCurrentTime()>currentEnd-1){
        if(loopMode==CLOUDS_LOOP){
            loadTime=socketHandler.eventHandler.events[2].startTime;
            currentEnd=socketHandler.eventHandler.events[2].startTime+socketHandler.eventHandler.events[2].duration;
            map.fadeIn(TRANSITION_CLOUDS);
        }
        else if(loopMode==GRADIENT_LOOP){
            loadTime=socketHandler.eventHandler.events[1].startTime;
            currentEnd=socketHandler.eventHandler.events[1].startTime+socketHandler.eventHandler.events[1].duration;
            map.fadeIn(TRANSITION_GRADIENT);
        }
        else if (loopMode==DEFAULT_LOOP){
            loadTime=socketHandler.eventHandler.events[5].startTime;
            currentEnd=socketHandler.eventHandler.events[5].startTime+socketHandler.eventHandler.events[5].duration;
            map.fadeIn(TRANSITION_DEFAULT);
        }
        else if (loopMode==WAVES_LOOP){
            loadTime=socketHandler.eventHandler.events[4].startTime;
            currentEnd=socketHandler.eventHandler.events[4].startTime+socketHandler.eventHandler.events[4].duration;
            map.fadeIn(TRANSITION_WAVES);
        }
        else if (loopMode==PARTY_LOOP){
            currentEnd=socketHandler.eventHandler.events[3].startTime+socketHandler.eventHandler.events[3].duration;
            loadTime=socketHandler.eventHandler.events[3].startTime;
            map.fadeIn(TRANSITION_PARTY);
        }
        bCheckingTime=false;
        ofSetWindowPosition(-1920,0);
        socketHandler.sendSocketCmd(END_REQ);
    }
    
    //transition has reached midpoint, load and start playing new position in video
    if(map.bTransitioning==true&&map.bTransitionLoading==false&&map.bTransitionStarted==false&&map.bTransitionFinished==false){
        MSA::ofxCocoa::setTime(loadTime);
        MSA::ofxCocoa::startPlayer();
        
        //start the pause period to allow glitchy loading to happen behind the fully opaque gradient transition texture
        map.bTransitionLoading=true;
        map.transitionTimer=ofGetElapsedTimeMillis();
        
        //start checking for end and pause points
        bCheckingTime=true;
    }
    
    //turn off cursor in performance mode
    if(map.bLocked==true){
        MSA::ofxCocoa::hideCursor();
        map.bLocked=false;
    }
    
    //show cursor when coming out of preformance mode
    else if(map.bUnlocked==true){
        MSA::ofxCocoa::showCursor();
        map.bUnlocked=false;
    }
    
    //check for pause point and if reached pause video
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
    
    
    //check for number of screens present
    if(MSA::ofxCocoa::getScreens()>=numScreens&&bScreenRestart==false){
        map.draw();
    }
    
    else if(MSA::ofxCocoa::getScreens()>=numScreens&&bScreenRestart==true){
        bStartScreenRestart=true;
    }
    
    else {
        bScreenRestart=true;
    }

    //if screens were not present and now are, restart app via socket command
    if(bScreenRestart==true&&bStartScreenRestart==true){
        socketHandler.sendSocketCmd(RESTART_REQ);
        bStartScreenRestart=false;
        bScreenRestart=false;
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
            
        //debug pause and position controls
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
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()-5);
            break;            
        case ']':
            MSA::ofxCocoa::setTime(MSA::ofxCocoa::getCurrentTime()+5);
            break;
            
        //debug quit app entirely and do not let node restart automatically
        case 'Q':
            socketHandler.sendSocketCmd(CLOSE_REQ);
            break;
    }
}

void testApp::exit(){
    
}

void testApp::initVariables(){

    // set default variable for default loop
    if(loopMode==DEFAULT_LOOP){
        currentEnd=socketHandler.eventHandler.events[5].startTime+socketHandler.eventHandler.events[5].duration;
        currentTransition=TRANSITION_DEFAULT;
        loadTime=socketHandler.eventHandler.events[5].startTime;
        map.fadeIn(TRANSITION_DEFAULT);
    }
    
    // set default variable for ambient loop
    else {
        currentEnd=socketHandler.eventHandler.events[2].startTime+socketHandler.eventHandler.events[2].duration;
        currentTransition=TRANSITION_CLOUDS;
        loadTime=socketHandler.eventHandler.events[2].startTime;
        map.fadeIn(TRANSITION_CLOUDS);
    }
    
    //Pause times for default and detail loops
    
    //Gastronomy Default
    pauseTimes.push_back(957.22);
    pauseTimes.push_back(963.26);
    pauseTimes.push_back(974.39);
    pauseTimes.push_back(981.02);
    //Markets Default
    pauseTimes.push_back(1003.32);
    pauseTimes.push_back(1010.10);
    pauseTimes.push_back(1020.33);
    pauseTimes.push_back(1026.05);
    //Shopping Default
    pauseTimes.push_back(1048.10);
    pauseTimes.push_back(1055.14);
    pauseTimes.push_back(1065.24);
    pauseTimes.push_back(1071.19);
    //Art & Design Default
    pauseTimes.push_back(1093.22);
    pauseTimes.push_back(1100.05);
    pauseTimes.push_back(1110.16);
    pauseTimes.push_back(1116.36);
    //Leisure Default
    pauseTimes.push_back(1138.01);
    pauseTimes.push_back(1145.05);
    pauseTimes.push_back(1155.30);
    pauseTimes.push_back(1161.16);
    
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
