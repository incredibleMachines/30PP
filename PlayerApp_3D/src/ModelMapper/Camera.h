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
    
    void set2D(int _meshNum, ofPoint translate, float scale, int horizGrid, int vertGrid, ofPoint texPos, int texWidth, int texHeight, float featherRight, float featherLeft, vector< vector<ofPoint> > originals, vector< vector<ofPoint> > warped);
    
    //-----------CAMERA DATA
    ofEasyCam camera;
    ofRectangle viewport;
    
    //-----------MESH OBJECT
    vector<ofMesh> mesh;
    
    struct texture2D{
    public:
        ofPoint pos;
        int width;
        int height;
    };

    struct meshObject{
    public:
        bool isMesh;
        texture2D tex;
        ofPoint translate;
        float scale;
        int horizGrid, vertGrid;
        vector< vector<ofPoint> > originals;
        vector< vector<ofPoint> > warped;
        float left;
        float right;
    };
    
    vector<meshObject> meshObjects;
    
    //-----------MASK OBJECTS
    vector<ofPolyline> masks;
    vector<ofPath> drawMasks;
    int highlightMask;
    
    vector<int> which;
    
};


