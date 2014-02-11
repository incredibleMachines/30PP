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
    
    textFile.setup("Eat it bitch");
    
    //QT Kit Video Player instantion
    background.setPixelFormat(OF_PIXELS_RGBA);
	ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
    background.loadMovie("mapping_test_04/tex/Test_Map_02.mov", decodeMode);
    background.play();
    
    drawSurface.allocate(background.getWidth(),background.getHeight(),GL_RGBA);
    textX=0;
    

}

void Composite::update(){
    background.update();
    textX++;
    ofDisableNormalizedTexCoords();
    ofEnableAlphaBlending();
    
    drawSurface.begin();
    ofClear(0, 0, 0,255);
    ofSetColor(255,255,255);
    ofCircle(800,300,20);
    
    ofPushMatrix();
    ofTranslate(textX, 200);
    textFile.draw();
    ofPopMatrix();
    
    background.draw(0,0);
    
    drawSurface.end();
    
    ofEnableNormalizedTexCoords();
    ofDisableAlphaBlending();
    
}

void Composite::bind(){
    drawSurface.getTextureReference().bind();
    
}

void Composite::unbind(){
    drawSurface.getTextureReference().unbind();
    
}