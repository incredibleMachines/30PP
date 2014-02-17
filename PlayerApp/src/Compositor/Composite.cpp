/*
 Composite class. Combines ofVboMesh text, ofImage images, ofQTKitPlayer content to create a single ofFbo texture for binding to a single mesh in ModelMapper.
 
 methods
 - setup(int meshNum) creates texture with correct texture coordinates and inits all globals
 - update() draws texture into ofFbo object.
 - bind()/unbind() wrapper for ofFbo bind/unbind, called from ModelMapper
 - drawFbo() draw all objects from SceneContent into Fbo in a appropriate positions determine by their scene and zone type
 - loadScene load new SceneContent data
 */

#include "Composite.h"

//--------------------------------------------------------------
void Composite::setup(int _meshNum){
    

    //testing - loading a single video texture directly from file
//    background.setPixelFormat(OF_PIXELS_RGBA);
//    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
//    background.loadMovie("UV_Sculpture_SexyMap.mov", decodeMode);
//    background.play();

    //----------TEXTURE SETUP
    
    //assign this texture to a specific mesh in ModelMapper
    meshNum=_meshNum;
    
    //load UV map for this mesh to get texture coordinates
    bgImage.loadImage("mapping_test/mesh_"+ofToString(meshNum)+".jpg");
    
    //setup Fbo (width, height, type, depth sample) currently set to use .maxSamples method of fbo to query graphics card on number of samples possible.
    drawSurface.allocate(bgImage.getWidth(),bgImage.getHeight(),GL_RGB,drawSurface.maxSamples());
    
    //set status variables
    bFinished=true;
    bLoaded=false;
    bPlaying=false;
    textPos=ofVec2f(0,200);
}

void Composite::update(){
    
    //testing - update background QTKitPlayer object
//    background.update();
    
    //--------UPDATE VIDEO TEXTURES
    
    //TODO make video textures load correctly, switch them to ofQTKitPlayer
    //TODO
    

    if(currentScene->sceneType==0){
        if(meshNum==2){
            pos.x=0;
            pos.y=0;
        }
        
        else if(meshNum==0){
            pos.x=-2850;
            pos.y=0;
        }
        
        else if(meshNum==1){
            pos.x=-1800;
            pos.y=400;
        }
    }
    
    else{
        pos.x=0;
        pos.y=0;
    }
    
    textPos.x+=5;
    
    if(textPos.x>4000){
        textPos.x=0;
    }
    
        drawFbo();
}

void Composite::bind(){
    drawSurface.getTextureReference().bind();
    
}

void Composite::unbind(){
    drawSurface.getTextureReference().unbind();
    
}

void Composite::drawFbo(){
    
    
    //----------CREATE TEXTURE
    
    //setup GL draw state for Fbo Texture
    ofDisableNormalizedTexCoords();
    ofEnableAlphaBlending();
    
    
    //drawFbo to reder buffer
    drawSurface.begin();
    
    //necessary to avoid video glitching at the start of all Fbos
    ofClear(0, 0, 0,255);
    ofSetColor(255,255,255);

    //testing - only draw video on sculpture mesh
//    if(meshNum==1){
//        background.draw(0,0);
//    }

    //check to make sure content has loaded before drawing
    if(bLoaded==true){
        
        //----------DRAW TEXT(S)
        
        ofPushMatrix();
        
        //TODO: this should be determined programatically by animation JSON
        ofTranslate(pos+textPos);
        for(int i=0; i<currentScene->txt.size();i++){
            currentScene->txt[i].text.draw();
        }
        
        ofPopMatrix();

        
        //----------DRAW IMAGE(S)
        
        ofPushMatrix();
        
        //TODO: this should be determined programatically by animation JSON
        ofTranslate(pos);
        
//        cout<<currentScene->img.size()<<endl;
        for(int i=0; i<currentScene->img.size();i++){
            currentScene->img[i].image.draw(0,0);
        }

        ofPopMatrix();
        

    
        //TODO: draw video textures
//        for(int i=0; i<currentScene->vid.size();i++){
//            currentScene->vid[i].video.draw(0,0);
//        }
    }
    
    drawSurface.end();
    
    //reset GL Context to default;
    ofEnableNormalizedTexCoords();
    ofDisableAlphaBlending();
}

void Composite::loadScene(SceneContent::meshScene &_currentScene){
    
    //reference meshScene for this scene, this mesh - passed by playerApp
    currentScene = &_currentScene;
    bLoaded=true;
    bPlaying=false;
}