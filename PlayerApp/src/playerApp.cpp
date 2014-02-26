#include "playerApp.h"

/*
 playerApp class. This is the master app, contains objects for Camera, ModelMapper, Masks, SocketHandler, Events.  
 
 methods
    - setup()  init all classes and json settings
    - update() update routine on socketHandler. Standard Operating Procedures after eventsInited. 
        - SOP: update modelMapper
    - draw()   socketDebugInfo to screen. Standard Operating Procedures after eventsInited
        - SOP: draw modelMapper
 */

//--- constants
#define MAPPER_DEBUG 0

    //--------------------------------------------------------------
void playerApp::setup(){
    
//----------OF GL SETUP
    ofSetVerticalSync(true);
    ofEnableDepthTest();
//    ofEnableSmoothing();
    ofSetFrameRate(30);
    ofEnableNormalizedTexCoords();
    ofBackground(0, 0, 0);
    ofSetWindowTitle("30PP Player");
    //setup mipmapping GL context
    ofEnableAntiAliasing();
    ofDisableArbTex();

    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    //----------SOCKET SETUP
    
    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
    //----------MODEL MAPPER SETUP
    

    //Load mesh vector to select which meshes within obj to use
    vector<int> _meshesLoad;
    _meshesLoad.push_back(0);
    _meshesLoad.push_back(1);

    numMesh=_meshesLoad.size();
    
    for(int i=0; i<MESH_NUM;i++){
        mesh[i].bSetup=false;
        mesh[i].loader.setPixelFormat(OF_PIXELS_RGB);
        mesh[i].loader.loadMovie("mesh/mesh_"+ofToString(i)+".mov");
        cout<<"loader"<<endl;
    }
    
    //setup ModelMapper - setup(number of Cameras, which camera is the gui, vector of mesh ids to draw)
    map.setup(4,0,_meshesLoad);
    
    //set path to obj file to use in setup
    map.setMassMesh("mesh/mesh.obj");
    
    //testing - set manual trigger to false
    bContentLoaded=false;
    bLoaded=true;
    play=0;
    load=0;
    frameCount=0;
    
}

//--------------------------------------------------------------
void playerApp::update(){
    
    //manage sockets, connect/reconnect as needed
    if(!MAPPER_DEBUG) socketHandler.update();
    
    if(socketHandler.eventHandler.eventsInited || MAPPER_DEBUG){   // we're good to go, follow standard operating procedures
        
        
        for(int i=0;i<MESH_NUM;i++){
            if(mesh[i].bSetup==false){
                setupTexture(i);
            }
        }
        
        if(bContentLoaded==true){
            
            if(frameCount==contentBuffer[play]->mesh[0]->frames&&frameCount!=0){
                
                play++;
                frameCount=0;
                
                if(play>BUFFER_SIZE-1){
                    play=0;
                }
                
                load=play-1;
                if(load<0){
                    load=BUFFER_SIZE-1;
                }
                
                bLoaded=false;
                cout<<"play"<<endl;
            }
            
            else if(frameCount>contentBuffer[play]->mesh[0]->frames/2&&bLoaded==false){
                
               
                //push new item into content buffer
                for(int j=0;j<MESH_NUM;j++){
                    contentBuffer[load]->mesh[j]->thread.unlock();
                    contentBuffer[load]->mesh[j]->thread.filepath=socketHandler.eventHandler.allScenes[count].sAssets[j].aFilePath;
                    cout<<"path: "<<contentBuffer[load]->mesh[j]->thread.filepath<<endl;
                    contentBuffer[load]->mesh[j]->thread.bRun=true;
                    contentBuffer[load]->mesh[j]->thread.bFinished=false;
                }
                
                bLoaded=true;
                
                count++;
                if(count>socketHandler.eventHandler.allScenes.size()-1){
                    count=0;
                }
                
            }
            
            for(int j=0;j<MESH_NUM;j++){
                if(contentBuffer[load]->mesh[j]->thread.bFinished==true){
                                    cout<<"submit"<<endl;
                    contentBuffer[load]->mesh[j]->pixels=contentBuffer[load]->mesh[j]->thread.pixels;
                    contentBuffer[load]->mesh[j]->thread.bFinished=false;
                    contentBuffer[load]->mesh[j]->frames=contentBuffer[load]->mesh[j]->thread.numFrames;
                    contentBuffer[load]->mesh[j]->thread.lock();
                }
            }
            
            for(int j=0;j<numMesh;j++){
                if(contentBuffer[play]->mesh[j]->thread.bLoaded==true){
                    createTexture(play,j);
                    if(j==0){
                        frameCount++;
                    }
                }
            }
        }
        
        map.update(meshTexture);
    
    }
    

    
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    //draw socket data onscreen - TODO: Move this inside model mapper so it will display on non-[0] guiCam and will not display in ADJUST_MODE_LOCKED
    
    socketHandler.drawDebugInfo(); //on screen socket debuggin
    
    if(socketHandler.eventHandler.eventsInited || MAPPER_DEBUG){ //we're good to go, follow SOP
        
        //Draw Everything
        
        bool setup=true;
        
        for(int i=0;i<numMesh;i++){
            if(mesh[i].bSetup==false){
                setup=false;
            }
        }
        
        if(setup==true){
            map.draw();
        }

    }
    ofSetColor(255);

}

//--------------------------------------------------------------
void playerApp::keyPressed(int key){
    
    //manually trigger init event
    if (key == 'i'){ // will happen in socketHandler.update() automatically when ready
        socketHandler.sendSocketCmd(INIT_REQ);
    }
    
    //manually trigger go event
    else if (key == 'o'){ // will happen in socketHandler.update() automatically when ready
        socketHandler.sendSocketCmd(GO_REQ);
    }
    
    //printout first loaded scene title
    else if (key == '.'){
        string test = socketHandler.eventHandler.allEvents[0].eScenes[1].sTitle;
        cout<< "event 0: scene 1: "+test<<endl;
    }
    
    //manually trigger all event content loading into contentBuffer
    else if(key == '0'){
        if(bContentLoaded==false){

            count=0;
            
            for(int i=0;i<BUFFER_SIZE;i++){
                contentBuffer.push_back(new videoScene());
                for(int j=0; j<MESH_NUM;j++){
                    contentBuffer[contentBuffer.size()-1]->mesh.push_back(new videoLoader());
                }
            }
            
            for(int i=0; i<BUFFER_SIZE;i++){
                for(int j=0;j<MESH_NUM;j++){
                    contentBuffer[i]->mesh[j]->thread.start();
                    contentBuffer[i]->mesh[j]->thread.filepath=socketHandler.eventHandler.allScenes[i].sAssets[j].aFilePath;
                    contentBuffer[i]->mesh[j]->thread.bRun=true;
                    contentBuffer[i]->mesh[j]->thread.bFinished=false;
                }
            }

        bContentLoaded=true;
        }
    }
    
    //manually switch to next scene. TODO: toss out previous contentBuffer, free memory
    else if (key== '='){
        
        
        
    }
}


//--------------------------------------------------------------
void playerApp::keyReleased(int key){

}

//--------------------------------------------------------------
void playerApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void playerApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void playerApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void playerApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void playerApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void playerApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void playerApp::dragEvent(ofDragInfo dragInfo){ 

}

void::playerApp::setupTexture(int _i){
        mesh[_i].loader.update();
        if(mesh[_i].loader.isLoaded()){
            for(int i=0;i<MESH_NUM;i++){
                meshTexture.push_back(new ofTexture());
            }
            //load pixel data to set our mipmapper texture
            
            mesh[_i].pix=new unsigned char[int(mesh[_i].loader.getWidth()*mesh[_i].loader.getHeight()*3)];
            mesh[_i].pix=mesh[_i].loader.getPixels();
            
            //allocate texture
            meshTexture[_i]->allocate(mesh[_i].loader.getWidth(), mesh[_i].loader.getHeight(), ofGetGlInternalFormat(mesh[_i].loader.getPixelsRef()) );
            
            mesh[_i].texData = meshTexture[_i]->texData;
//            
//            //save format and type as globals
            mesh[_i].glFormat=ofGetGlFormat(mesh[_i].loader.getPixelsRef());
            mesh[_i].glType=ofGetGlType(mesh[_i].loader.getPixelsRef());
            mesh[_i].width=mesh[_i].loader.getWidth();
            mesh[_i].height=mesh[_i].loader.getHeight();
            
//            cout<<"Format:"<<glFormat<<endl;
//            cout<<"Type:"<<glType<<endl;
            
            //special case for texture type - we will always be using GL_2D but just in case
            if (mesh[_i].texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
                mesh[_i].texData.tex_t = mesh[_i].loader.getPixelsRef().getWidth();
                mesh[_i].texData.tex_u = mesh[_i].loader.getPixelsRef().getHeight();
            } else {
                mesh[_i].texData.tex_t = (float)(mesh[_i].loader.getPixelsRef().getWidth()) / (float)mesh[_i].texData.tex_w;
                mesh[_i].texData.tex_u = (float)(mesh[_i].loader.getPixelsRef().getHeight()) / (float)mesh[_i].texData.tex_h;
            }
            
            //set our pixel source to determine mip map texel size
            ofSetPixelStorei(mesh[_i].loader.getPixelsRef().getWidth(),mesh[_i].loader.getPixelsRef().getBytesPerChannel(),mesh[_i].loader.getPixelsRef().getNumChannels());
            
            //create texture
            glGenTextures(1, &mesh[_i].texData.textureID);
            
            //bind texture
            glBindTexture(mesh[_i].texData.textureTarget, mesh[_i].texData.textureID);
            
            //setup mipmaps
            glTexImage2D(mesh[_i].texData.textureTarget, 0, mesh[_i].texData.glTypeInternal, mesh[_i].loader.getWidth(), mesh[_i].loader.getHeight(), 0, mesh[_i].glFormat, mesh[_i].glType, mesh[_i].pix);
            glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
            
            //set environment, not using currenty but just incase we change our env elsewhere
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            
            //enable GL_TEXTURE_2D
            glEnable(  mesh[_i].texData.textureTarget);
            
            //create mipmaps
            glGenerateMipmap(mesh[_i].texData.textureTarget);
            
            //set params, GL_LINEAR_MIPMAP_LINER is trilinear which means greatest quality
            glTexParameteri( mesh[_i].texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri( mesh[_i].texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            
            //anisotropy unblurs our mip maps, basically sets "how much" we want antialias with higher param being less blur
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
            glDisable( mesh[_i].texData.textureTarget );
            
            mesh[_i].loader.close();
            mesh[_i].bSetup=true;
            cout<<"load"<<endl;
    }
}

void playerApp::createTexture(int _i, int _j){
    
    
    //----------CREATE TEXTURE
    
    ofSetColor(255,255,255);
    
        //save current frame pixel data to a pixel array for loading into glTexImage2D
        mesh[_j].pix=contentBuffer[_i]->mesh[_j]->pixels[frameCount];
    
        //set other glTexImage2D variables
    
        //pair with current texture
        mesh[_j].texData = meshTexture[_j]->texData;
    
        //bind texture
        glBindTexture(mesh[_j].texData.textureTarget, mesh[_j].texData.textureID);
    
        //setup mipmap context
        glEnable(GL_TEXTURE_2D);
    
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mesh[_j].width, mesh[_j].height, mesh[_j].glFormat, mesh[_j].glType, mesh[_j].pix);
    
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        
        //environment, in case we change elsewhere
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        //enable GL_TEXTURE_2D
        glEnable(  mesh[_j].texData.textureTarget);
        
        //make mipmaps - REQUIRES GL to be post-3.0
        glGenerateMipmap(mesh[_j].texData.textureTarget);
    
        //set quality - GL_LINEAR_MIPMAP_LINEAR is highest, trilinear
        glTexParameteri( mesh[_j].texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri( mesh[_j].texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        //set amount of anisotropic filtering - this undoes the blur of the mipmapping relative to camera
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
        glDisable( mesh[_j].texData.textureTarget );
    
}

void playerApp::exit(){
    for(int i=0;i<BUFFER_SIZE;i++){
        for(int j=0;j<MESH_NUM;j++){
//            contentBuffer[i]->mesh[j]->thread.stop();
        }
    }
}


