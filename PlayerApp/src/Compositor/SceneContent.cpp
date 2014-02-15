/*
 SceneContent class. Loads content into memory for a particular scene, making vectors of content for each mesh. Receives instructions and filepaths for content from EventHandler and Asset classes.
 
 methods
 - load(Scene, int numMeshes) takes Scene data and load content into memory, loading ofImage and ofVideoPlayer assets and generation ofVbo meshes of text strings.
 */

#include "SceneContent.h"
#include "playerApp.h"

void SceneContent::load(Scene * scene, int _numMeshes){
    
    //----------CREATE SCENE
    
    //create vector for all meshes
    meshScene tempMesh;
    for(int i=0;i<_numMeshes;i++){
        fullScene.push_back(tempMesh);
    }
    
    //----------LOAD CONTENT
    
    for(int i=0;i<scene->sAssets.size();i++){
        
        //assign Asset to mesh. TODO: make this a look-up of which zone goes with which mesh
        currentMesh=scene->sAssets[i].aZone;
        
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
        
        //if fileType is image, load images
        if(scene->sAssets[i].aType==2){
            ImageContent tempImage;
            tempImage.image.loadImage(scene->sAssets[i].aFilePath);
            if(currentMesh<ZONE_FULL_SCREEN){
                tempImage.fullScreen=true;
            }
            fullScene[currentMesh].img.push_back(tempImage);
            
        }
        
        //if fileType is video, load video. TODO: make this ofQTKitPlayer instead of ofVideoPlayer, solve .play() on this content
        else if(scene->sAssets[i].aType==1){
            VideoContent tempVid;
            tempVid.video.loadMovie(scene->sAssets[i].aFilePath);
            if(currentMesh<ZONE_FULL_SCREEN){
                tempVid.fullScreen=true;
            }
            fullScene[currentMesh].vid.push_back(tempVid);
        }
    }
}