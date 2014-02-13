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
    ImageFile();
    ImageFile(string _filepath);
    
    void update();
    void draw();
        
    ofImage img;
};