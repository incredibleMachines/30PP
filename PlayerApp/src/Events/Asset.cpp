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
 
*/

#include "Asset.h"
#include "playerApp.h"

//--------------------------------------------------------------
Asset::Asset(Json::Value thisAsset){
    
    //--- get the zone #. it is the same as the member name.
    string assetZone = thisAsset.getMemberNames()[0];
    aZone           = ofToInt(assetZone);
    //cout << "assetZone: " << aZone << endl;
    
    //--- pull out the core of this asset object
    thisAsset = thisAsset.get(assetZone, "did not find asset #"+assetZone);
    //cout << "thisAsset: "<< thisAsset << endl;
    
    //--- ready to go, pull out rest of asset metadata
    aCaption        = thisAsset.get("caption","null").asString();
    aType           = thisAsset.get("type","no asset type found").asInt();
    //aTitle          = thisAsset.get("title","no asset title found").asString(); //title is now in file{}
    
    cout<<"\t\t\t>> assetType: "<<aType<<"\taCaption: "<<aCaption<<"\taZone: "<<aZone<<endl;
    
    //--- get File
    aFile           = thisAsset.get("file", "did not find any file in this asset");
    //cout << "thisFile: "<< aFile << endl;
    
    if (aFile.isObject()){
        
        //--- ready to go, pull out all file metadata
        aFileTitle      = aFile.get("title", "no file title found").asString();
//        aFileLoc.address = aFile.get("location", "no file location found").get("address", "no file loc address found").asString();
//        aFileLoc._id    = aFile.get("location", "no file location found").get("_id", "no file loc _id found").asString();
        aFilePath       = aFile.get("path","no file path found").asString();
        aFileType       = aFile.get("type","no file path found").asInt();
        aFileCreatedAt  = aFile.get("created_at", "no file created at found").asString();
        aFile_id        = aFile.get("_id", "no file _id found").asString();
        cout<<"\t\t\t\t>> fileTitle: "<< aFileTitle << "\tfilePath: "<< aFilePath << "\tfileType: "<< aFileType << "\tfileType: "<< aFileType;
    }   else cout<<"\t\t\t\t>>No file found in this asset"<< endl;
    
    


    
    //--- file pathing!
    /* NOTE: this path is relative to the 30PP folder, meaning
    finalFilePath begins as pointing to: User/..../openFrameworks/apps/30PP/  like so:  */
    
    aFilePath = "../../../" + aFilePath; //append URI to base file path (30PP root folder)
    cout<<"complete asset filepath: "<< aFilePath << endl;  //print out completed file path


    
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
//    switch (aType){
//            
//    }
    
}
