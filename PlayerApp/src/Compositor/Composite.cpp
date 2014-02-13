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
//    background.setPixelFormat(OF_PIXELS_RGBA);
//	ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
//    background.loadMovie("Mapping_test_06/UV_SCULPT_02h264.mov", decodeMode);
//    background.play();
    bgImage.loadImage("WALL_MAP_02/WALL MAP_02.jpg");
    drawSurface.allocate(bgImage.getWidth(),bgImage.getHeight(),GL_RGBA);
//    drawSurface.allocate(background.getWidth(),background.getHeight(),GL_RGBA);
    cout<<"SETUP"<<endl;
    bFinished=true;
    bLoaded=false;
    bPlaying=false;
    drawSurface.getTextureReference().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
}

void Composite::update(){
    background.update();
    textX++;
    drawFbo();
    
        if(bLoaded==true){
            for(int i=0; i<currentScene->vid.size();i++){
                if(!currentScene->vid[i].video.isPlaying()){
                    currentScene->vid[i].video.play();
                }
                else{
                    if(currentScene->vid[i].video.isPlaying()){
                        currentScene->vid[i].video.update();
                    }
                }
            }
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

    bool bFullScreen=false;
    
//    if(bLoaded==true){
//        
//        ofPushMatrix();
//        
//        ofTranslate(200,200);
//        for(int i=0; i<currentScene->txt.size();i++){
//            currentScene->txt[i].text.draw();
//        }
//        
//        ofPopMatrix();
//        
//        ofPushMatrix();
//    
//        for(int i=0; i<currentScene->img.size();i++){
//            currentScene->img[i].image.draw(0,0);
//            if(currentScene->img[i].fullScreen==true){
//                bFullScreen=true;
//            }
//        }
//        
//        ofPopMatrix();
//        
//        for(int i=0; i<currentScene->vid.size();i++){
//            currentScene->vid[i].video.draw(0,0);
//            if(currentScene->vid[i].fullScreen==true){
//                bFullScreen=true;
//            }
//        }
//    }
    
    if(bFullScreen==false){
//        background.draw(0,0);

    }
    
                bgImage.draw(0,0);
    
    drawSurface.end();
    ofEnableNormalizedTexCoords();
    ofDisableAlphaBlending();
}

void Composite::loadScene(SceneContent::meshScene &_currentScene){
    currentScene = &_currentScene;
    bLoaded=true;
    bPlaying=false;
}

void Composite::initScene(){

}