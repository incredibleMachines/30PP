//
//  Scene.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/31/14.
//
//

#include "Scene.h"

//--------------------------------------------------------------
Scene::Scene(Json::Value thisScene){
    
    //cout<< "SCENE RECEIVED: "<<thisScene.toStyledString()<<endl; //entire raw event
    
    sType           = thisScene.get("type", "no type found").asString();
    sTitle          = thisScene.get("title", "no title found").asString();
    int numAssets   = thisScene.get("assets", "NO ASSETS FOUND").size();
    
    cout<<endl<<"\t\t>> sceneType: "<<sType<<"\t sceneTitle: "<<sTitle<<"\t numAssets: "<<numAssets<<endl;
    
    
    Json::Value assets = thisScene.get("assets", "assets object not found");

    for (int i=0; i<numAssets; i++){
        Asset thisAsset = * new Asset(assets[i]);
        sAssets.push_back(thisAsset);
    }
}

//--------------------------------------------------------------
void Scene::update(){
    
}

//--------------------------------------------------------------
void Scene::draw(){
    
    
}