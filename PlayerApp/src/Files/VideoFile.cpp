//
//  VideoFile.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/3/14.
//
/*
 
This is the object that will hold a VIDEO File for any Asset that requires one.
 
 methods:
    - constructor accepts filepath, caption (may come in null), and target/zone coordinates of the Video. Checks to see if caption exists.
    - update() can hold any manipulations/animations to video sizing, positioning. Also contains vidPlayer.update()
    - draw() draws videos to coords and size (TODO: create vars for this)
 
*/

#include "VideoFile.h"

//--------------------------------------------------------------
VideoFile::VideoFile(){
    
}


VideoFile::VideoFile(string _filepath, string _caption, ofVec2f _coords){
    
    filePath = _filepath;
    caption = _caption; //might be null
    coords = _coords;
    
    vid.loadMovie(filePath);
    
    if (caption.length() > 1){
        
        //TODO: create a TextFile, set a flag to also update the image file going forward.
        
    }
    
//     player.loadMovie(<#string path#>)
}

//--------------------------------------------------------------
void VideoFile::update(){
    
    //coords can be updated here if needed
    
    vid.update();
}

//--------------------------------------------------------------
void VideoFile::draw(){
    
    vid.draw(coords);
}