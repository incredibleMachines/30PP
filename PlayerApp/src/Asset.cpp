//
//  Asset.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/6/14.
//
//

#include "Asset.h"

//--------------------------------------------------------------
Asset::Asset(string _eventId, string _title, string _type, string _location, string _uri){ //x,y,w,h in later?
    
    eventId = _eventId;
    type = _type;
    title = _title;
    uri = _uri;
    location = _location;
    
    x = 0;
    y = 0;
    width = 960;
    height = 360;
    
    //TODO: generate this path automatically
    string filePath = "/Users/jmsaavedra/Documents/openFrameworks/openFrameworks_0080_osx/apps/30PP/";
    
    filePath += uri; //append URI to base file path (30PP root folder)
    
//if (type == "video"){
    
    vid.loadMovie(filePath);
    
    vid.play();
//}
}

//--------------------------------------------------------------
void Asset::update(){
    vid.update();
}

//--------------------------------------------------------------
void Asset::draw(){
    
    vid.draw(x, y, width, height);
    
}