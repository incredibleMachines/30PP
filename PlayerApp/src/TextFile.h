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
    TextFile(string _text, ofVec2f _coords);
    
    void update();
    void setup();
    void draw();
    
    
    string text;
    ofVec2f coords;
    
    ofTrueTypeFont ttFont;
    
};