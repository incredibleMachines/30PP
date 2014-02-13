//
//  Asset.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/6/14.
//
//
/*

 Asset file class. Assets are stored inside of a vector in every Scene.
 
 methods:
 
    - constructer pulls out all metadata from asset JSON object, including type, zone, caption, and file. If file object exists, pulls all metadata at this moment. At most, only ever 1 file per asset is possible, so makes sense to do this here.
    - update() updates this asset. will only ever contain 1 type of file. (if it has an img or video AND ALSO has text, TextFile is generated and updated from the VideoFile or ImageFile class. This is determined by checking the caption
    - draw() draws this asset.
 
*/

#include "Asset.h"
#include "playerApp.h"

//--------------------------------------------------------------
Asset::Asset(Json::Value thisAsset){
    
    //--- pull out all asset metadata
    aType           = thisAsset.get("type","no asset type found").asInt();
    aZone           = thisAsset.get("zone","no asset zone found").asInt();
    aCaption        = thisAsset.get("caption","null").asString();
    aFilePath       = thisAsset.get("link","no file path found").asString();
    
    // NOTE: might move these to file class
    aLoc.address    = thisAsset.get("location","no asset location found").get("address","no asset loc address found").asString();
    aLoc.title      = thisAsset.get("location","no asset location found").get("title","no asset loc title found").asString();
    aTitle          = thisAsset.get("title","no asset title found").asString();
    
    cout<<"\t\t\t>> assetType: "<<aType<<"\t\tassetTitle: "<<aTitle<<"\taCaption: "<<aCaption<<"\taZone: "<<aZone<<"\tLoc.title: "<<aLoc.title<<"\tLoc.address: "<<aLoc.address<<endl;
    
    //--- file pathing!
    // NOTE: this path is relative to the 30PP folder, meaning
    // finalFilePath begins as pointing to: User/..../openFrameworks/apps/30PP/  like so:
//    finalFilePath = "../../../";
//    finalFilePath += aFilePath; //append URI to base file path (30PP root folder)
//    cout<<"complete asset filepath: "<< finalFilePath << endl;  //print out completed file path
//

    
    //--- assign asset coordinates by zoneID
    switch (aZone){
        //TODO: define this cases
        case 0:
            aCoords = ofVec2f(10, 10);
        break;
            
        case 1:
            aCoords = ofVec2f(210, 350);
        break;
        case 2:
            aCoords = ofVec2f(450, 65);
        break;
        default: //wtf
            aCoords = ofVec2f(500,500);
        break;
    }
    
    //TODO: check fileType and assign asset dimensions (w,h) based on this. add w,h to constructor of file
    
    //--- now create the correct type of asset file
    switch (aType){
            
    }
    
}

//--------------------------------------------------------------
void Asset::update(){
    
}

//--------------------------------------------------------------
void Asset::draw(){

}