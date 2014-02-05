//
//  Mask.cpp
//  3camTest
//
//  Created by IncredibleMachines on 2/5/14.
//
//

#include "Mask.h"

void Mask::setup(){
    
}

void Mask::draw(){
    ofSetColor(0,0,0);
    maskShape.draw();
}

void Mask::addVertex(ofVec2f vertex){
    maskShape.addVertex(vertex);
}