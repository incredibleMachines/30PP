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
    
    cout<<numMesh<<endl;
    
    for(int i=0;i<_numMeshes;i++){
        tempMesh.sceneType=scene->sZoneType;
        fullScene.push_back(tempMesh);
    }
    
    //----------LOAD CONTENT
    
    for(int i=0;i<scene->sAssets.size();i++){
        
        //assign Asset to mesh. TODO: make this a look-up of which zone goes with which mesh
        
        currentMesh=scene->sAssets[i].aZone;
        
        loadText(i);
        
        //if fileType is image, load images
        cout<<scene->sAssets[i].aFileType<<endl;
        if(scene->sAssets[i].aFileType==2){
            cout<<"image"<<endl;
            ImageContent tempImage;
            tempImage.image.loadImage(scene->sAssets[i].aFilePath);
            if(currentMesh<ZONE_FULL_SCREEN){
                tempImage.fullScreen=true;
            }
            fullScene[currentMesh].img.push_back(tempImage);
        }
        
        //if fileType is video, load video. TODO: make this ofQTKitPlayer instead of ofVideoPlayer, solve .play() on this content
        else if(scene->sAssets[i].aFileType){
            VideoContent tempVid;
            tempVid.video.loadMovie(scene->sAssets[i].aFilePath);
            if(currentMesh<ZONE_FULL_SCREEN){
                tempVid.fullScreen=true;
            }
            fullScene[currentMesh].vid.push_back(tempVid);
        }
    }
}

void SceneContent::loadText(int i){
    //TODO - add if to determine whether we want to load text all
    //create ofVboMesh of text - Always do this
    ttFont = &((playerApp*)ofGetAppPtr())->masterFont;
    vector<ofTTFCharacter> shapes = ttFont->getStringAsPoints(scene->sAssets[i].aCaption);
    TextContent tempText;
    for(int j = 0; j < shapes.size(); j++) {
        ofMesh cur = shapes[j].getTessellation();
        tempText.text.setMode(cur.getMode());
        int indexOffset = tempText.text.getVertices().size();
        tempText.text.addVertices(cur.getVertices());
        for(int k = 0; k < cur.getIndices().size(); k++) {
            tempText.text.addIndex(indexOffset + cur.getIndices()[k]);
        }
        
    }
    fullScene[currentMesh].txt.push_back(tempText);

}

void SceneContent::loadImage(int i){
    
}

void SceneContent::loadVideo(int i){
    
}