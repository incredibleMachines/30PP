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
#include "playerApp.h"


//--------------------------------------------------------------
TextFile::TextFile(){
    
}

//--------------------------------------------------------------
TextFile::TextFile(string _text, ofVec2f _coords){
    
    //TODO: choose font lib to load text into
    textString = _text;
    coords = _coords;
    
    // From Kyle - https://github.com/openframeworks/openFrameworks/issues/1891
    
    ttFont = &((playerApp*)ofGetAppPtr())->masterFont;
    
    vector<ofTTFCharacter> shapes = ttFont->getStringAsPoints(textString);
    // load a vector<ofTTFCharacter> into an ofMesh
    for(int i = 0; i < shapes.size(); i++) {
        ofMesh cur = shapes[i].getTessellation();
        text.setMode(cur.getMode());
        int indexOffset = text.getVertices().size();
        text.addVertices(cur.getVertices());
        for(int j = 0; j < cur.getIndices().size(); j++) {
            text.addIndex(indexOffset + cur.getIndices()[j]);
        }
    }
}

//--------------------------------------------------------------
void TextFile::setup(string _text){
    textString = _text;
    
    
}


//--------------------------------------------------------------
void TextFile::update(){
    
    //update coordinates, coloring as per any animations if needed
}

//--------------------------------------------------------------
void TextFile::draw(){
    ofPushMatrix();
    ofTranslate(coords);
    text.draw();
    ofPopMatrix();
    
}

//--------------------------------------------------------------