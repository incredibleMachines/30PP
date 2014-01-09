//
//  Event.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#include "Event.h"

//--------------------------------------------------------------
Event::Event(int eventIndex, ofxLibwebsockets::Event &args){
    
    cout << "->creating event number: "<< eventIndex << " -------------------------------"<<endl;
    
    int numAssets = args.json["events"].get(eventIndex, "event not found").get("assets", "asset not found").size();

    eAnimType   = args.json["events"].get(eventIndex, "event not found").get("anim_type", "anim_type not found").asString();
    eId         = args.json["events"].get(eventIndex, "event not found").get("_id", "id not found").asString();
    eTitle      = args.json["events"].get(eventIndex, "event not found").get("title", "title not found").asString();
    eDuration   = args.json["events"].get(eventIndex, "event not found").get("duration", "duration not found").asString();
    eCreatedAt  = args.json["events"].get(eventIndex, "event not found").get("created_at", "created not found").asString();
    
    //print EVENT keys
    cout << "\t>>> eID: " << eId << "\t||\teTitle: "<< eTitle << "\t||\tnumAssets: "<< numAssets<< "\t||\teDur: " << eDuration << "\t||\teCreatedAt: " << eCreatedAt << endl;
    
    
    //go through every asset in this event and push it into this eventAssets vector
    for (int i=0; i < numAssets; i++){

        string aUri   = args.json["events"].get(eventIndex, "event not found").get("assets", "assets not found").get(i, "asset i not found").get("path", "path not found").asString();
        string aTitle = args.json["events"].get(eventIndex, "event not found").get("assets", "assets not found").get(i, "asset i not found").get("title", "title not found").asString();
        string aType  = args.json["events"].get(eventIndex, "event not found").get("assets", "assets not found").get(i, "asset i not found").get("type", "type not found").asString();
        string aLocation = args.json["events"].get(eventIndex, "event not found").get("assets", "assets not found").get(i, "asset i not found").get("location", "location not found").asString();

        
        // print ASSET keys
        cout<< "\t>>> asset #: "<< i << "\taTitle: "<< aTitle << "\taUri: "<< aUri <<"\taType: "<< aType << "\taLoc: "<< aLocation << endl;
        
        Asset thisAsset = * new Asset (aTitle, aType, aLocation, aUri); // pointer to temp Asset
        eAssets.push_back(thisAsset); //push into eventAssets vector
    }
    cout << "\t>>> eventAssets.size = "<<eAssets.size()<<endl;
}

//--------------------------------------------------------------
//void Event::initEvent(){ } // can be moved into here if needed
