//
//  Syphons.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#include "Syphons.h"

Syphons::Syphons(){
    
    
    
}

void Syphons::setup(){
    
    for (int i = 0; i < NUM_TEXTURES; i++ ){
        server[i].setName("Texture Out " + ofToString(i));
        tex[i].allocate(640, 480, GL_RGBA);
    }
}

void Syphons::draw(){
    // draw static into our one texture.
    unsigned char pixels[640*480*4];
    for (int i = 0; i < 640*480*4; i++) {
        pixels[i] = (int)(255 * ofRandomuf());
    }
    tex[0].loadData(pixels, 640, 480, GL_RGBA);
    server[0].publishTexture(&tex[0]);
    
    publishAll();
}

void Syphons::publishAll(){
    
    for (int i=0; i< NUM_TEXTURES; i++){
        server[i].publishTexture(&tex[i]);
        
    }
}