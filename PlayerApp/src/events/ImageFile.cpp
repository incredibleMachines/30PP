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

ImageFile::ImageFile(string _filepath, string _caption, ofVec2f _coords){
    
    filePath = _filepath;
    caption = _caption; //might be null
    coords = _coords;
    
    img.loadImage(filePath);
    
    if (caption.length() > 1){
        
        //TODO: create a TextFile, set a flag to also update the image file going forward.
        
    }
    
}

//--------------------------------------------------------------
void ImageFile::update(){
    
    //update coords as per animations here, if any
    
}

//--------------------------------------------------------------
void ImageFile::draw(){
    
    img.draw(coords);
}