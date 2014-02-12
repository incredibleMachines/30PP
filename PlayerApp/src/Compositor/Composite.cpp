//
//  Composite.cpp
//  PlayerApp
//
//  Created by IncredibleMachines on 2/10/14.
//
//


#include "Composite.h"

//--------------------------------------------------------------
void Composite::setup(int _meshNum){
    meshNum=_meshNum;
    background.setPixelFormat(OF_PIXELS_RGBA);
	ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
    background.loadMovie("Mapping_test_06/UV_SCULPT_02h264.mov", decodeMode);
    background.play();
    drawSurface.allocate(background.getWidth(),background.getHeight(),GL_RGBA);
    bFinished=true;
}

void Composite::update(){
        background.update();
    for(int i=0;i<videoFiles.size();i++){
        
    }
    textX++;
    drawFbo();
//    if(ofGetElapsedTimeMillis()-timer>5000)
//    {
//        bFinished=true;
//    }
    
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
    
    ofPushMatrix();

    for(int i=0; i<textFiles.size();i++){
        textFiles[i]->draw();
    }
    
    ofPopMatrix();
    
    background.draw(0,0);
    
    drawSurface.end();
    
    ofEnableNormalizedTexCoords();
    ofDisableAlphaBlending();
}

void Composite::loadScene(Scene _scene){
    bFinished==false;
    
    //QT Kit Video Player instantion
    timer=ofGetElapsedTimeMillis();
    currentScene=_scene;
    initScene();
}

void Composite::initScene(){
    textFiles.clear();
    imageFiles.clear();
    videoFiles.clear();
    for(int i=0;i<currentScene.sAssets.size();i++){
        TextFile * tempText=currentScene.sAssets[i].txtFile;
        textFiles.push_back(tempText);
    }
}