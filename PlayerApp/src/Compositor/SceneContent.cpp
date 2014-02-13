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
        ttFont = &((playerApp*)ofGetAppPtr())->masterFont;
        vector<ofTTFCharacter> shapes = ttFont->getStringAsPoints(scene->sAssets[i].aCaption);
        // load a vector<ofTTFCharacter> into an ofMesh
        
        TextContent tempText;
        
        for(int j = 0; j < shapes.size(); j++) {
            ofMesh cur = shapes[j].getTessellation();
            tempText.text.setMode(cur.getMode());
            int indexOffset = tempText.text.getVertices().size();
            tempText.text.addVertices(cur.getVertices());
            for(int k = 0; k < cur.getIndices().size(); k++) {
                tempText.text.addIndex(indexOffset + cur.getIndices()[k]);
            }
            fullScene[scene->sAssets[i].aZone].txt.push_back(tempText);
        }
        
            if(scene->sAssets[i].aType==2){
                ImageContent tempImage;
                tempImage.image.loadImage(scene->sAssets[i].aFilePath);
                fullScene[scene->sAssets[i].aZone].img.push_back(tempImage);
                
            }
            
            else if(scene->sAssets[i].aType==1){
                VideoContent tempVid;

                tempVid.video.loadMovie(scene->sAssets[i].aFilePath);
                cout<<scene->sAssets[i].aFilePath<<endl;
                fullScene[scene->sAssets[i].aZone].vid.push_back(tempVid);
            }
    }
}