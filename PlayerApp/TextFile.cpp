//
//  TextFile.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/3/14.
//
//

/*
 
 This is the object that will hold a TEXT File for any Asset that requires one.
 
 methods:
    - constructor accepts filepath, caption (may come in null), and target/zone coordinates of the Video.
    - update() updates coordinates, text sizing, coloring that could be animated
    - draw() sets color, draws text to screen
 
 */

#include "TextFile.h"


//--------------------------------------------------------------
TextFile::TextFile(string _text, ofVec2f _coords){
    
    //TODO: choose font lib to load text into
    text = _text;
    coords = _coords;
}

//--------------------------------------------------------------
void TextFile::update(){
    
    //update coordinates, coloring as per any animations if needed
}

//--------------------------------------------------------------
void TextFile::draw(){
    
    //TODO: use font framework/addon as chosen
    
    ofSetColor(200); //for now
    ofDrawBitmapString(text, coords); //for now
}

//--------------------------------------------------------------