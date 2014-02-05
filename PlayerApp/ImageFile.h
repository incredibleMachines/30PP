//
//  ImageFile.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 2/3/14.
//
//

#pragma once
#include "ofMain.h"


class ImageFile {
    
public:
    ImageFile(string _filepath, string _caption, ofVec2f _coords);
    
    void update();
    void draw();
    
    string filePath;
    string caption;
    ofVec2f coords;
    
    ofImage img;
};