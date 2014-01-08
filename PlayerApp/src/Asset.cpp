//
//  Asset.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/6/14.
//
//

#include "Asset.h"

//--------------------------------------------------------------
Asset::Asset(string _title, string _type, string _location, string _uri){ //x,y,w,h in later?
    
    type = _type;
    title = _title;
    uri = _uri;
    location = _location;
    
    x = 0;
    y = 0;
    width = 960;
    height = 360;

    vid.loadMovie(uri);
    vid.play();
}

//--------------------------------------------------------------
void Asset::update(){
    vid.update();
}

//--------------------------------------------------------------
void Asset::draw(){
    
    vid.draw(x, y, width, height);
    
}