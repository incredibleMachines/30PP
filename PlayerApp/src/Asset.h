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
};


// Asset object
class Asset {
    
    public:
        //Asset(string _eventId, string _title, string _type, string _location, string _uri);
        Asset(Json::Value thisAsset);
        void update();
        void draw();
        
        string aTitle;
        string aUri;
        string aType;
        Location aLoc;
        
        int x;
        int y;
        int width;
        int height;
        
        ofVideoPlayer vid;
    //    ofFile file;
        
    private:

};