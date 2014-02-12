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
#include "../Files/VideoFile.h"
#include "../Files/ImageFile.h"
#include "../Files/TextFile.h"


// Location object
class Location {
    public:
        string address;
        string title;
};


// Asset object
class Asset {
    
    public:
    Asset();
        Asset(Json::Value thisAsset);
        void update();
        void draw();
        int aType;      // assetType
        int aZone;          // dictates coordinates
        ofVec2f aCoords;    // dicated by aZone (ID int)
        string aCaption;
        string aTitle;      // might move
        Location aLoc;      // might move
    
        //--- Vars for a File, if found
        Json::Value aFile;  // assetFile JSON
        string aFilePath;   // rest comes in aFile JSON obj
        string aFileTitle;
        string aFileText;
        string aFileMongoId;
        int aFileType;
        string aFileCreatedAt;
        Location aFileLoc;
        string finalFilePath;
    
        VideoFile * vidFile; //pointers so as to not allocate any memory when not needed
        ImageFile * imgFile;
        TextFile * txtFile;
    
        int width;  //future?
        int height; //future?

    
    private:

};