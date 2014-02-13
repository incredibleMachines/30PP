//
//  ImageFile.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/3/14.
//
//
/*
 
 This is the object that will hold an IMAGE File for any Asset that requires one.
 
 methods
    - constructor loads image from filepath, defines coordinates. Checks to see if there is a caption.
    - update() can hold any translations, fading, manipulations of image color, size, coords.
    - draw() draws image to screen.
 
 */

#include "ImageFile.h"

//--------------------------------------------------------------

ImageFile::ImageFile(){
    
}

//--------------------------------------------------------------

ImageFile::ImageFile(string _filepath){
    
    img.loadImage(_filepath);
    
    
}

//--------------------------------------------------------------
void ImageFile::update(){
    
}

//--------------------------------------------------------------
void ImageFile::draw(){
    img.draw(0,0);
}