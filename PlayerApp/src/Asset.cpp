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

//--------------------------------------------------------------
Asset::Asset(Json::Value thisAsset){
    
    //--- pull out all asset metadata
    aType           = thisAsset.get("type","no asset type found").asInt();
    aZone           = thisAsset.get("zone","no asset zone found").asInt();
    aCaption        = thisAsset.get("caption","null").asString();
    
    // NOTE: might move these to file class
    aLoc.address    = thisAsset.get("location","no asset location found").get("address","no asset loc address found").asString();
    aLoc.title      = thisAsset.get("location","no asset location found").get("title","no asset loc title found").asString();
    aTitle          = thisAsset.get("title","no asset title found").asString();
    
    cout<<"\t\t\t>> assetType: "<<aType<<"\t\tassetTitle: "<<aTitle<<"\taCaption: "<<aCaption<<"\taZone: "<<aZone<<"\tLoc.title: "<<aLoc.title<<"\tLoc.address: "<<aLoc.address<<endl;
    
    
    //--- check to see if there is a file in this asset

    if (!thisAsset.get("file", 0).isInt() ) {   // found a file !!
        
        cout<<" FILE FOUND IN THIS ASSET "<< endl;
        cout << thisAsset.get("file", "no asset file found").type() <<endl;
        
        //--- pull out and store JSON 'file' object in aFile
        aFile           = thisAsset.get("file","NO asset FILE FOUND WHAT LASTCHECKFAILED?!");
        
        //--- pull out all aFile metadata
        aFileMongoId    = aFile.get("_id","no file _id found").asString();
        aFileTitle      = aFile.get("title","no file title found").asString();
        aFileText       = aFile.get("text","no file text found").asString();
        aFileType       = aFile.get("type","no file type found").asDouble();
        aFileCreatedAt  = aFile.get("created_at","no file created_at found").asString();
        aFileLoc.address= aFile.get("location","no file location found").get("address","no file loc addr found").asString();
        aFileLoc.title  = aFile.get("location","no file location found").get("title","no file loc title found").asString();
        aFilePath       = aFile.get("path","no file path found").asString();
        
        //--- file pathing!
        // NOTE: this path is relative to the 30PP folder, meaning
        // finalFilePath begins as pointing to: User/..../openFrameworks/apps/30PP/  like so:
        finalFilePath = "../../../";
        finalFilePath += aFilePath; //append URI to base file path (30PP root folder)
        cout<<"complete asset filepath: "<< finalFilePath << endl;  //print out completed file path
    }
    
    
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
            aCoords = ofVec2f(0,0);
        break;
    }
    
    //TODO: check fileType and assign asset dimensions (w,h) based on this. add w,h to constructor of file
    
    //--- now create the correct type of asset file
    switch (aFileType){

        case 0:  //--- TEXT
            txtFile = new TextFile(aCaption, aCoords);                    //TODO: uncomment
            txtFile->setup();
        break;
        case 1:  //--- VIDEO
            //vidFile = new VideoFile(finalFilePath, aCaption, aCoords);    //TODO: uncomment
        break;
        case 2:  //--- IMAGE
            //imgFile = new ImageFile(finalFilePath, aCaption, aCoords);    //TODO: uncomment
        break;
        default: //wtf case
            ;
    }
    
}



//--------------------------------------------------------------
void Asset::update(){
    
    switch (aType){
        case 0:  //TEXT
            txtFile->update();
        break;
        case 1:  //VIDEO
            //vidFile->update();
        break;
        case 2:  //IMAGE
            //imgFile->update();
        break;
        default: //wtf case
            //wtf
        break;
    }
}

//--------------------------------------------------------------
void Asset::draw(){
    
    switch (aType){
        case 0:  //TEXT
            txtFile->draw();
        break;
        case 1:  //VIDEO
            //vidFile->draw();
        break;
        case 2:  //IMAGE
            //imgFile->draw();
        break;
        default: //wtf case
            ;
    }
    
}