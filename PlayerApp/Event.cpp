//
//  Event.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#include "Event.h"

Event::Event(ofxLibwebsockets::Event &args){

    eCommand = args.json["command"].asString();
    eTitle = args.json["title"].asString();
    eDuration = args.json["duration"].asString();
    
    cout << "eCmd: " << eCommand << "    eTitle: "<< eTitle << "    eDur: " << eDuration << endl;
    
    for (int i=0; i < args.json["assets"].size(); i++){
        
        // cout<< "asset #: "<< i << "   data: "<< args.json["assets"].get(i, "title") << endl; // full asset object print out

        Asset thisAsset = * new Asset;
        thisAsset.aLink = args.json["assets"].get(i, "asset not found").get("link", "link not found").asString();
        thisAsset.aTitle = args.json["assets"].get(i, "asset not found").get("title", "link not found").asString();
        thisAsset.aType = args.json["assets"].get(i, "asset not found").get("type", "link not found").asString();
        thisAsset.aLocation = args.json["assets"].get(i, "asset not found").get("location", "link not found").asString();

        cout<< "asset #: "<< i << "   aTitle: "<< thisAsset.aTitle << "   aLink: "<< thisAsset.aLink <<"   aType: "<< thisAsset.aType << "   aLoc: "<< thisAsset.aLocation << endl;
        
        eAssets.push_back(thisAsset);
    }
    
    cout<<"total assets: "<<eAssets.size()<<endl;
    cout <<"======================="<<endl;
}

//void Event::initEvent(){ }
