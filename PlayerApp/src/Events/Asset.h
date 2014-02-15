//
//  Asset.h
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/6/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxLibwebsockets.h"


// Location object
class Location {
    public:
        string address;
        string title;
        string _id;
};


// Asset object
class Asset {
    
    public:
        Asset(Json::Value thisAsset);
        int aType;      // assetType
        int aZone;          // dictates coordinates
        ofVec2f aCoords;    // dicated by aZone (ID int)
        string aCaption;
        string aTitle;      // might move
    
        //--- Vars for a File, if found
        Json::Value aFile;  // assetFile JSON
        string aFilePath;   // rest comes in aFile JSON obj
        string aFileTitle;
        string aFileText;
        string aFile_id;
        int aFileType;
        string aFileCreatedAt;
        Location aFileLoc;
    
    
        int width;  //future?
        int height; //future?

    
    private:

};