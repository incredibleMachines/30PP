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
    meshScene tempMesh;
    numMesh=_numMeshes;
    
    
    for(int i=0;i<_numMeshes;i++){
        fullScene.push_back(tempMesh);
    }
    
    //----------LOAD CONTENT
    
    for(int i=0;i<scene->sAssets.size();i++){
        
        //assign Asset to mesh. TODO: make this a look-up of which zone goes with which mesh
        
        currentMesh=scene->sAssets[i].aZone;
        
        if(scene->sAssets[i].aFileType==1){
            ofVideoPlayer tempVid;
            fullScene[currentMesh].vid.push_back(tempVid);
            fullScene[currentMesh].vid[fullScene[currentMesh].vid.size()-1].loadMovie(scene->sAssets[i].aFilePath);
        }
    }
}