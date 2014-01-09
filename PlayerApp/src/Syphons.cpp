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
void Syphons::setup( vector<Asset> &_pAssets){
    
    pAsset = &_pAssets; //ref to allAssets from playerApp
    
    numAssets = pAsset->size();
    
    for (int i = 0; i < numAssets; i++ ){
        server[i].setName("Texture Out " + ofToString(i));
        //tex[i].allocate(960, 360, GL_RGBA);
        fbo[i].allocate(960, 360, GL_RGBA32F_ARB);  // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
        
        fbo[i].begin();
        ofClear(255,255,255,0); // clear out graphic card artifacts from Fbo
        fbo[i].end();
    }
    
    cout << "*** syphons setup, pAsset size: " << numAssets << endl;
}

//--------------------------------------------------------------
void Syphons::update(){

    //cout<< "total number of Assets: "<< numAssets << endl;
    
    for (int i=0; i < numAssets; i++){
        
        //--- update asset first
        pAsset->at(i).update(); //seems to run faster than:
        //(*pAsset)[i].update(); //alt reference
        
//        //--- fill Fbo
//        fbo[i].begin();                 //--- start fbo
//        
//        //--- draw asset
//        pAsset->at(i).draw();
//        //(*pAsset)[i].draw();
//        
//        fbo[i].end();                   //--- end fbo
    }
    
    //cout<<thisVid.getPixelFormat() << endl;
}

//--------------------------------------------------------------
void Syphons::draw(){
    
    
    for (int i=0; i < numAssets; i++){
        
        //--- update asset first
        //pAsset->at(i).update(); //seems to run faster than:
        //(*pAsset)[i].update(); //alt reference
        
        //--- fill Fbo
        fbo[i].begin();                 //--- start fbo
        
        //--- draw asset
        pAsset->at(i).draw();
        //(*pAsset)[i].draw();
        
        fbo[i].end();                   //--- end fbo
    }
    
/*  texture tests:
 
    // draw static into our one texture.
    unsigned char pixels[960*360*4];
    for (int i = 0; i < 960*360*4; i++) {
        pixels[i] = (int)(255 * ofRandomuf());
    }
    tex[0].loadData(pixels, 960, 360, GL_RGBA);
    server[0].publishTexture(&tex[0]);
    
    //thisVid.getTextureReference().bind();
    //vboMesh.draw();
    //thisVid.getTextureReference().bind();
    
    ofPixels px = thisVid.getPixelsRef();
    tex[1].loadData(px, GL_RGB);
    //server[1].publishTexture(&thisVid.getTextureReference());
    server[1].publishTexture(&tex[1]);  */
    
    publishAll();
}

//--------------------------------------------------------------
void Syphons::publishAll(){
    
    for (int i=0; i< numAssets; i++){
        
        server[i].publishTexture( &fbo[i].getTextureReference() );
        
        //server[i].publishTexture(&tex[i]);
        //server[0].publishTexture(&vboMesh.);
    }
}


