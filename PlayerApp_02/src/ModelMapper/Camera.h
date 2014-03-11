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
    
        //-----------MASK OBJECTS
        vector<ofPolyline> masks;
        vector<ofPath> drawMasks;
        int highlightMask;
};


