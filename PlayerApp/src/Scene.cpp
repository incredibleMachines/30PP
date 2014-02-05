//
//  Scene.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/31/14.
//
//
/*

 Scene class. Scenes are stored in a vector that is part of every Event.
 
 methods:
    - constructor: pulls all metadata associated with a Scene including the zone arrangement type. Populates a vector of Asset objects, that are associated with this particular Scene.
    - update() iterates through all Assets of this scene and updates all every time it's called
    - draw() iterates through all Assets, draws them.
*/

#include "Scene.h"

//--------------------------------------------------------------
Scene::Scene(Json::Value thisScene){
    
    //cout<< "SCENE RECEIVED: "<<thisScene.toStyledString()<<endl; //print entire raw scene
    
    sZoneType       = thisScene.get("type", "no scene type found").asDouble();
    sTitle          = thisScene.get("title", "no scene title found").asString();
    sText           = thisScene.get("text", "no scene text found").asString();
    sCreatedAt      = thisScene.get("created_at", "no scene created_at").asString();
    int numAssets   = thisScene.get("assets", "NO SCENE ASSETS FOUND").size();
    
    cout<<endl<<"\t sceneTitle: "<<sTitle<<"\t>> zoneType: "<<sZoneType<<"\t numAssets: "<<numAssets<<endl;
    
    //--- pull out assets object (array) for this scene
    Json::Value assets = thisScene.get("assets", "assets object not found");

    //--- populate this scene's assets vector
    for (int i=0; i<numAssets; i++){
        Asset thisAsset = * new Asset(assets[i]);
        sAssets.push_back(thisAsset);
    }
}

//--------------------------------------------------------------
void Scene::update(){
    
    for (int i=0; i<sAssets.size(); i++) {
        sAssets[i].update();
    }
}

//--------------------------------------------------------------
void Scene::draw(){
    
    for (int i=0; i<sAssets.size(); i++) {
        sAssets[i].draw();
    }
}