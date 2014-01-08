//
//  Syphons.cpp
//  PlayerApp
//
//  Created by Joseph Saavedra on 1/2/14.
//
//

#include "Syphons.h"
#include "playerApp.h"

Syphons::Syphons(){
    
}

//--------------------------------------------------------------
void Syphons::setup(vector <Asset> &_pAssets){
    
    for (int i = 0; i < NUM_SYPHONS; i++ ){
        server[i].setName("Texture Out " + ofToString(i));
        //tex[i].allocate(960, 360, GL_RGBA);
        fbo[i].allocate(960, 360, GL_RGBA32F_ARB);  // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
        
        fbo[i].begin();
        ofClear(255,255,255,0); // clear out graphic card artifacts from Fbo
        fbo[i].end();
    }
    
    pAsset = &_pAssets; //ref to allAssets from playerApp
    
    cout << "** syphons setup, pAsset size: " << pAsset->size() << endl;
}

//--------------------------------------------------------------
void Syphons::update(){
    
    int totalAssetCount = ((playerApp*) ofGetAppPtr())->allAssets.size();
    Event currentEvent = ((playerApp*) ofGetAppPtr())->allEvents.back();

    cout<< " currentEvent.eAssets.size() "<< currentEvent.eAssets.size() << endl;
    
    for(int i=0; i < currentEvent.eAssets.size(); i++){ //how many assets in this event
        for (int j=0; j<totalAssetCount; j++){ //go through all assets
            cout<< i << " : "<< currentEvent.eAssets[i].title << endl;
            if (((playerApp*) ofGetAppPtr())->allAssets[j].title == currentEvent.eAssets[i].title){ //compare titles - NOT EFFICIENT AT ALL
                ((playerApp*) ofGetAppPtr())->allAssets[j].update();
            }
        }
    }
    
    for (int i=0; i < NUM_SYPHONS; i++){
        
        //--- update asset first
        //((playerApp*) ofGetAppPtr())->allAssets[0].update(); //TODO: pass in pointer manually
        //allAssets[i].update();
        
        
        //--- fill Fbo
        fbo[i].begin();                 // start fbo
        
        //pseudo code:
        //if(asset[i].location
        
        ((playerApp*) ofGetAppPtr())->allAssets[0].draw();    // draw vid in it
        //allAssets[i].draw(0, 0, 960, 360);
        
        fbo[i].end();                   // end fbo
    }
    
    //cout<<thisVid.getPixelFormat() << endl;
}

//--------------------------------------------------------------
void Syphons::draw(){
//    // draw static into our one texture.
//    unsigned char pixels[960*360*4];
//    for (int i = 0; i < 960*360*4; i++) {
//        pixels[i] = (int)(255 * ofRandomuf());
//    }
//    tex[0].loadData(pixels, 960, 360, GL_RGBA);
//    server[0].publishTexture(&tex[0]);
    
    //thisVid.getTextureReference().bind();
    //vboMesh.draw();
    //thisVid.getTextureReference().bind();
    
//    ofPixels px = thisVid.getPixelsRef();
//    tex[1].loadData(px, GL_RGB);
//    //server[1].publishTexture(&thisVid.getTextureReference());
//    server[1].publishTexture(&tex[1]);
    
    publishAll();
}

//--------------------------------------------------------------
void Syphons::publishAll(){
    
    for (int i=0; i< NUM_SYPHONS; i++){
        
        server[i].publishTexture( &fbo[i].getTextureReference() );
        
        //server[i].publishTexture(&tex[i]);
        //server[0].publishTexture(&vboMesh.);
    }
}


