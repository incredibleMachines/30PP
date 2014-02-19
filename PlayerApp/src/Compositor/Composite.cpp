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
    
    //----------TEXTURE SETUP
    
    //assign this texture to a specific mesh in ModelMapper
    meshNum=_meshNum;
    
    //load UV map for this mesh to get texture coordinates
    bgImage.loadImage("mapping_test/mesh_"+ofToString(meshNum)+".jpg");
    
    //setup Fbo (width, height, type, depth sample) currently set to use .maxSamples method of fbo to query graphics card on number of samples possible.
    drawSurface.allocate(bgImage.getWidth(),bgImage.getHeight(),GL_RGB,8);
    
    //set status variables
    bFinished=true;
    bLoaded=false;
    bPlaying=false;
    textPos=ofVec2f(0,200);
}

void Composite::update(){
    
    //--------UPDATE VIDEO TEXTURES
    
    //TODO make video textures load correctly, switch them to ofQTKitPlayer
    
    if(bPlaying==false&&bLoaded==true){
        if(currentScene->vid.size()!=0){
            for(int i=0; i<currentScene->vid.size();i++){
                currentScene->vid[i].play();
            }
        }
        bPlaying=true;
    }
    if(bPlaying==true){
        
        for(int i=0; i<currentScene->vid.size();i++){
            currentScene->vid[i].update();
        }
    }
    
    pos.x=0;
    pos.y=0;
    
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
        for(int i=0; i<currentScene->img.size();i++){
            currentScene->img[i].image.draw(0,0);
        }

        ofPopMatrix();
        

    
        //TODO: draw video textures
        ofPushMatrix();
        ofTranslate(pos);
        for(int i=0; i<currentScene->vid.size();i++){
            cout<<"video width:"<< currentScene->vid[i].getMoviePath()<<endl;
            currentScene->vid[i].draw(0,0);
        }
        ofPopMatrix();
    }
    
    drawSurface.end();
    
    //reset GL Context to default;
    ofEnableNormalizedTexCoords();
    ofDisableAlphaBlending();
}

void Composite::loadScene(SceneContent::meshScene * _currentScene){
    
    //reference meshScene for this scene, this mesh - passed by playerApp
    currentScene = _currentScene;
    bLoaded=true;
    bPlaying=false;
}