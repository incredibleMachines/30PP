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
#define MAPPER_DEBUG 1

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
    
//    socketHandler.setup(8080, true); // (PORT,  bool verboseMode)
    
    //----------MODEL MAPPER SETUP
    

    //Load mesh vector to select which meshes within obj to use
    vector<int> _meshesLoad;
    _meshesLoad.push_back(0);
    _meshesLoad.push_back(1);
    _meshesLoad.push_back(2);

    numMesh=_meshesLoad.size();
    
    for(int i=0; i<MESH_NUM;i++){
        mesh[i].bSetup=false;
        mesh[i].video.setPixelFormat(OF_PIXELS_RGB);
        mesh[i].video.initPlayer(ofToString(i)+"_0.mp4");
        mesh[i].video.play();
        cout<<"playerApp init"<<endl;
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
//    if(!MAPPER_DEBUG) socketHandler.update();
    
//    if(socketHandler.eventHandler.eventsInited || MAPPER_DEBUG){   // we're good to go, follow standard operating procedures
//        
//
    if(bContentLoaded==false){
        for(int i=0;i<MESH_NUM;i++){
            mesh[i].video.update();
            if(mesh[i].video.isLoaded()==true&&mesh[i].bSetup==false){
                cout<<"make first texture"<<endl;
                setupTexture(i);
                mesh[i].bSetup=true;
            }
            else if(mesh[i].bSetup==true){
                createTexture(0,i);
            }
        }
    }
    
    map.update(meshTexture);
    
//    }
    

    
}

//--------------------------------------------------------------
void playerApp::draw(){
    
    bool setup=true;
        
        for(int i=0;i<numMesh;i++){
            if(mesh[i].bSetup==false){
                setup=false;
            }
        }
        
        if(setup==true){
            map.draw();
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
//        if(bContentLoaded==false){
//
//            count=0;
//            
//                for(int j=0;j<numMesh;j++){
//                    string frames[3];
//                    frames[0]="UV_"+ofToString(j)+"_0.mp4";
//                    frames[1]="UV_"+ofToString(j)+"_1.mp4";
//                    frames[2]="UV_"+ofToString(j)+"_2.mp4";
//                    mesh[j].video.initPlayer(frames);
//                    mesh[j].loaded=false;
//                }
//
//        bContentLoaded=true;
//        }
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

void playerApp::setupTexture(int _i){

                meshTexture.push_back(new ofTexture());
//          load pixel data to set our mipmapper texture
//          mesh[_i].pix=new unsigned char[int(mesh[_i].video.getWidth()*mesh[_i].video.getHeight()*3)];
    
            mesh[_i].pix=mesh[_i].video.getPixels();
            
            //allocate texture
            meshTexture[_i]->allocate(mesh[_i].video.getWidth(), mesh[_i].video.getHeight(), ofGetGlFormat(mesh[_i].video.getPixelsRef()) );
            
//            mesh[_i].texData = meshTexture[_i]->texData;
//
//            //save format and type as globals
            mesh[_i].glFormat=ofGetGlFormat(mesh[_i].video.getPixelsRef());
            mesh[_i].glType=ofGetGlType(mesh[_i].video.getPixelsRef());
            mesh[_i].width=mesh[_i].video.getWidth();
            mesh[_i].height=mesh[_i].video.getHeight();
    
//            cout<<"Format:"<<glFormat<<endl;
//            cout<<"Type:"<<glType<<endl;
    
            //special case for texture type - we will always be using GL_2D but just in case
            if (meshTexture[_i]->texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
                meshTexture[_i]->texData.tex_t = mesh[_i].video.getPixelsRef().getWidth();
                meshTexture[_i]->texData.tex_u = mesh[_i].video.getPixelsRef().getHeight();
            }
    
            else {
                meshTexture[_i]->texData.tex_t = (float)(mesh[_i].video.getPixelsRef().getWidth()) / (float)meshTexture[_i]->texData.tex_w;
                meshTexture[_i]->texData.tex_u = (float)(mesh[_i].video.getPixelsRef().getHeight()) / (float)meshTexture[_i]->texData.tex_h;
            }
            
            //set our pixel source to determine mip map texel size
            ofSetPixelStorei(mesh[_i].video.getPixelsRef().getWidth(),mesh[_i].video.getPixelsRef().getBytesPerChannel(),mesh[_i].video.getPixelsRef().getNumChannels());
            
            //create texture
            glGenTextures(1, &meshTexture[_i]->texData.textureID);
            
            //bind texture
            glBindTexture(meshTexture[_i]->texData.textureTarget, meshTexture[_i]->texData.textureID);
            
            //setup mipmaps
            glTexImage2D(meshTexture[_i]->texData.textureTarget, 0, meshTexture[_i]->texData.glTypeInternal, mesh[_i].video.getWidth(), mesh[_i].video.getHeight(), 0, mesh[_i].glFormat, mesh[_i].glType, mesh[_i].pix);
            glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
            
            //set environment, not using currenty but just incase we change our env elsewhere
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            
            //enable GL_TEXTURE_2D
            glEnable(  meshTexture[_i]->texData.textureTarget);
            
            //create mipmaps
            glGenerateMipmap(meshTexture[_i]->texData.textureTarget);
            
            //set params, GL_LINEAR_MIPMAP_LINER is trilinear which means greatest quality
            glTexParameteri( meshTexture[_i]->texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri( meshTexture[_i]->texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            
            //anisotropy unblurs our mip maps, basically sets "how much" we want antialias with higher param being less blur
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
            glDisable( meshTexture[_i]->texData.textureTarget );
            
//            mesh[_i].video.setPaused(true);
//            mesh[_i].video.close();
            mesh[_i].bSetup=true;
            cout<<"texture loaded"<<endl;
}

void playerApp::createTexture(int _i, int _j){
    
    //----------CREATE TEXTURE
    
    ofSetColor(255,255,255);
    
        //save current frame pixel data to a pixel array for loading into glTexImage2D
        mesh[_j].pix=mesh[_j].video.getPixels();
    
        //set other glTexImage2D variables
    
    
        //bind texture
        glBindTexture(meshTexture[_j]->texData.textureTarget, meshTexture[_j]->texData.textureID);
    
        //setup mipmap context
        glEnable(GL_TEXTURE_2D);
    
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mesh[_j].width, mesh[_j].height, mesh[_j].glFormat, mesh[_j].glType, mesh[_j].pix);
    
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        
        //environment, in case we change elsewhere
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        //enable GL_TEXTURE_2D
        glEnable(  meshTexture[_j]->texData.textureTarget);
        
        //make mipmaps - REQUIRES GL to be post-3.0
        glGenerateMipmap(meshTexture[_j]->texData.textureTarget);
    
        //set quality - GL_LINEAR_MIPMAP_LINEAR is highest, trilinear
        glTexParameteri( meshTexture[_j]->texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri( meshTexture[_j]->texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        //set amount of anisotropic filtering - this undoes the blur of the mipmapping relative to camera
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
        glDisable( meshTexture[_j]->texData.textureTarget );
    
}

void playerApp::exit(){
    for(int i=0;i<BUFFER_SIZE;i++){
        for(int j=0;j<MESH_NUM;j++){
//            contentBuffer[i]->mesh[j]->thread.stop();
        }
    }
}


