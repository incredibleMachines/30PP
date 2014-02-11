//
//  TextFile.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/3/14.
//
//

#pragma once
#include "ofMain.h"

class TextFile {
    
public:
    TextFile();
    TextFile(string _text, ofVec2f _coords);
    
    void update();
    void setup();
    void setup(string _text);
    void draw();
    
    
    string textString;
    ofVec2f coords;
    
    ofTrueTypeFont ttFont;
    ofVboMesh text;
    
};