//
//  Event.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#include "Event.h"

//--------------------------------------------------------------
Event::Event(ofxLibwebsockets::Event &args){

    eCommand = args.json["command"].asString();
    eTitle = args.json["title"].asString();
    eDuration = args.json["duration"].asString();
    
    //print EVENT keys
    cout << "eCmd: " << eCommand << "    eTitle: "<< eTitle << "    eDur: " << eDuration << endl;
    
    for (int i=0; i < args.json["assets"].size(); i++){
        // cout<< "asset #: "<< i << "   data: "<< args.json["assets"].get(i, "default") << endl; // full asset object print out


        string aUri      = args.json["assets"].get(i, "asset not found").get("link", "link not found").asString();
        string aTitle    = args.json["assets"].get(i, "asset not found").get("title", "title not found").asString();
        string aType     = args.json["assets"].get(i, "asset not found").get("type", "type not found").asString();
        string aLocation = args.json["assets"].get(i, "asset not found").get("location", "loc not found").asString();

        
        // print ASSET keys
        cout<< "asset #: "<< i << "\taTitle: "<< aTitle << "\taUri: "<< aUri <<"\taType: "<< aType << "\taLoc: "<< aLocation << endl;
        
        //now happens in createAssets in playerApp class
        //Asset thisAsset = * new Asset (aTitle, aType, aLocation, aUri); // pointer to temp Asset
        //eAssets.push_back(thisAsset); //push into eventAssets vector
    }
    
    cout<<"total assets in Event: "<<args.json["assets"].size()<<endl; // completed parsing event
}

//--------------------------------------------------------------
//void Event::initEvent(){ } // can be moved into here if needed
