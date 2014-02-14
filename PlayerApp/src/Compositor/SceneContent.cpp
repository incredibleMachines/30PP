//
//  SceneContent.cpp
//  PlayerApp
//
//  Created by IncredibleMachines on 2/12/14.
//
//

#include "SceneContent.h"
#include "playerApp.h"

void SceneContent::load(Scene * _scene, int _numMeshes){
    scene=_scene;
    meshScene tempMesh;
    for(int i=0;i<_numMeshes;i++){
        fullScene.push_back(tempMesh);
    }
    
    for(int i=0;i<scene->sAssets.size();i++){
        currentMesh=scene->sAssets[i].aZone;
        ttFont = &((playerApp*)ofGetAppPtr())->masterFont;
        vector<ofTTFCharacter> shapes = ttFont->getStringAsPoints(scene->sAssets[i].aCaption);
        
        cout << scene->sAssets[i].aCaption << endl;
        
        // load a vector<ofTTFCharacter> into an ofMesh
        
        TextContent tempText;
        
        cout << "shapes.size(): "<< shapes.size() << endl;
        
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
        
        
            if(scene->sAssets[i].aType==2){
                ImageContent tempImage;
                tempImage.image.loadImage(scene->sAssets[i].aFilePath);
                if(currentMesh<ZONE_FULL_SCREEN){
                    tempImage.fullScreen=true;
                }
                fullScene[currentMesh].img.push_back(tempImage);
                
            }
            
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