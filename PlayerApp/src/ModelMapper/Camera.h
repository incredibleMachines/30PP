//
//  Camera.h
//  3camTest
//
//  Created by IncredibleMachines on 1/30/14.
//
//

#pragma once
#include "ofMain.h"

class Camera {
public:
    
    //Setup passes through data parsed from JSON in ModelMapper
    void setup(ofVec3f _pos, ofQuaternion _orientation, ofVec3f _viewPos, ofVec3f _viewSize, vector<ofMesh> _mesh, vector<ofPolyline> _masks);
    
    //create additional mask
    void addMask();
    
    //-----------CAMERA DATA
    ofEasyCam camera;
    ofRectangle viewport;
    
    //-----------MESH OBJECT
    vector<ofMesh> mesh;
    
    struct texture2D{
        ofPoint pos;
        int width;
        int height;
    };
    
    struct meshObject{
        bool isMesh;
        ofMesh mesh;
        texture2D tex;
        ofPoint translate;
        int horizPoints, vertPoints;
        vector<ofPoint> pointsOrig;
        vector<ofPoint> pointsCurr;
    };
    
    //-----------MASK OBJECTS
    vector<ofPolyline> masks;
    vector<ofPath> drawMasks;
    int highlightMask;
};


