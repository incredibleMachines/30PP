//
//  Composite.cpp
//  PlayerApp
//
//  Created by IncredibleMachines on 2/10/14.
//
//


#include "Composite.h"

//--------------------------------------------------------------
void Composite::setup(){
    background.setPixelFormat(OF_PIXELS_RGBA);
	ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
    background.loadMovie("Mapping_test_06/UV_SCULPT_02h264.mov", decodeMode);
    background.play();
    drawSurface.allocate(background.getWidth(),background.getHeight(),GL_RGBA);
    cout<<"SETUP"<<endl;
    bFinished=true;
    bLoaded=false;
    bPlaying=false;
}

void Composite::update(){
    background.update();
    textX++;
    drawFbo();
    
        if(bLoaded==true){
            for(int i=0; i<currentScene->vid.size();i++){
                if(bPlaying==false){
                    currentScene->vid[i].video.play();
                }
                else{
                    if(currentScene->vid[i].video.isPlaying()){
                        currentScene->vid[i].video.update();
                    }
                }
            }
            bPlaying=true;
        }
}

void Composite::bind(){
    drawSurface.getTextureReference().bind();
    
}

void Composite::unbind(){
    drawSurface.getTextureReference().unbind();
    
}

void Composite::drawFbo(){
    ofDisableNormalizedTexCoords();
    ofEnableAlphaBlending();
    
    drawSurface.begin();
    ofClear(0, 0, 0,255);
    ofSetColor(255,255,255);
    ofCircle(800,300,20);
    
    if(bLoaded==true){
        
        ofPushMatrix();
        
        ofTranslate(200,200);
        for(int i=0; i<currentScene->txt.size();i++){
            currentScene->txt[i].text.draw();
        }
        
        ofPopMatrix();
        
        ofPushMatrix();
        
        for(int i=0; i<currentScene->img.size();i++){
            currentScene->img[i].image.draw(0,0);
        }
        
        ofPopMatrix();
        
        for(int i=0; i<currentScene->vid.size();i++){
            currentScene->vid[i].video.draw(500,500);
        }
    }
    
    background.draw(0,0);
    
    drawSurface.end();
    ofEnableNormalizedTexCoords();
    ofDisableAlphaBlending();
}

void Composite::loadScene(SceneContent::meshScene &_currentScene){
    currentScene = &_currentScene;
    bLoaded=true;
}

void Composite::initScene(){

}