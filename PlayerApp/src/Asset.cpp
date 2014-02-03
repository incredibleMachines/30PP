//
//  Asset.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/6/14.
//
//

#include "Asset.h"

//--------------------------------------------------------------
Asset::Asset(Json::Value thisAsset){ //x,y,w,h in later?
    
    aTitle          = thisAsset.get("title","no asset title found").asString();
    aType           = thisAsset.get("type","no asset type found").asString();
    aUri            = thisAsset.get("link","no asset link found").asString();
    aLoc.address    = thisAsset.get("location","no asset location found").get("address","no loc address found").asString();
    aLoc.title      = thisAsset.get("location","no asset location found").get("title","no loc title found").asString();
    
    x = 0;
    y = 0;
    width = 960;
    height = 360;
    
    cout<<"\t\t\t>> assetType: "<<aType<<"\tassetTitle: "<<aTitle<<"\taUri: "<<aUri<<"\tLoc.title: "<<aLoc.title<<"\tLoc.address: "<<aLoc.address<<endl;
    
    //TODO: generate this path automatically
    string filePath = "/Users/jmsaavedra/Documents/openFrameworks/openFrameworks_0080_osx/apps/30PP/";
    
//    filePath += uri; //append URI to base file path (30PP root folder)
//  if (type == "video"){
//    vid.loadMovie(filePath);
//    vid.play();
//  }
}

//--------------------------------------------------------------
void Asset::update(){
    vid.update();
}

//--------------------------------------------------------------
void Asset::draw(){
    
    vid.draw(x, y, width, height);
    
}


////--------------------------------------------------------------
//Asset::Asset(string _eventId, string _title, string _type, string _location, string _uri){ //x,y,w,h in later?
//    
//    eventId = _eventId;
//    type = _type;
//    title = _title;
//    uri = _uri;
//    location = _location;
//    
//    x = 0;
//    y = 0;
//    width = 960;
//    height = 360;
//    
//    //TODO: generate this path automatically
//    string filePath = "/Users/jmsaavedra/Documents/openFrameworks/openFrameworks_0080_osx/apps/30PP/";
//    
//    filePath += uri; //append URI to base file path (30PP root folder)
//    
//    //if (type == "video"){
//    
//    vid.loadMovie(filePath);
//    
//    vid.play();
//    //}
//}
//