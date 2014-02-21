/*
 SceneContent class. Loads content into memory for a particular scene, making vectors of content for each mesh. Receives instructions and filepaths for content from EventHandler and Asset classes.
 
 methods
 - load(Scene, int numMeshes) takes Scene data and load content into memory, loading ofImage and ofVideoPlayer assets and generation ofVbo meshes of text strings.
 */

#include "SceneContent.h"
#include "playerApp.h"

void SceneContent::load(Scene * _scene, int _numMeshes){
    
    //----------CREATE SCENE
    
    scene=_scene;
    //create vector for all meshes
    numMesh=_numMeshes;
    
    
    for(int i=0;i<_numMeshes;i++){
        fullScene.push_back(new ofxAVFVideoPlayer());
    }
    
    //----------LOAD CONTENT
    
    for(int i=0;i<2;i++){
        
        //assign Asset to mesh
        
        currentMesh=scene->sAssets[i].aZone;
        
        cout<<"loading mesh: "<<currentMesh<<endl;
        
        if(scene->sAssets[i].aFileType==1){
            cout<<"loading mesh: "<<currentMesh<<endl;
            cout<<"total mesh: "<<fullScene.size()<<endl;
//            fullScene[currentMesh]->setPixelFormat(OF_PIXELS_RGBA);
            fullScene[currentMesh]->loadMovie(scene->sAssets[i].aFilePath);
            fullScene[currentMesh]->setLoopState(OF_LOOP_NORMAL);
        }
    }
}

void SceneContent::update(){
//    for(int i=0;i<fullScene.size();i++){
//        fullScene[i].update();
//    }
}