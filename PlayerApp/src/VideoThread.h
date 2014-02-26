#pragma once

//openFrameworks + addons
#include "ofMain.h"

//class videoEvent : public ofEventArgs {
//public:
//    bool loaded;
//    videoEvent() {
//        loaded = NULL;
//    }
//    static ofEvent<videoEvent> events;
//};


class VideoThread : public ofThread {
public:
    
    ofQTKitPlayer * video;
    vector<unsigned char *> pixels;
    bool bRun;
    bool bFinished;
    bool bLoading;
    bool bFirst;
    bool bLoaded;
    string filepath;
    int numMesh;
    int bufferSize;
    int currentSlot;
    float numFrames;
    ofQTKitDecodeMode decodeMode;
    
    VideoThread(){
        bRun=false;
        bFinished=false;
        bLoading=false;
        bFirst=true;
        bLoaded=false;
        decodeMode=OF_QTKIT_DECODE_PIXELS_ONLY;
    };

    void threadedFunction(){
        while(isThreadRunning() !=0){
            if(bRun==true){
                if(lock()){
                    video->stop();
                    video->close();
                    delete video;
                    video=new ofQTKitPlayer();
                    bRun=false;
                    video->loadMovie(filepath,decodeMode);
                    bLoading=true;
                    bFirst=false;
                    numFrames=-10;
                    for(int i=0;i<pixels.size();i++){
                        delete pixels[i];
                    }
                    pixels.clear();
                }
            }
            if(numFrames==video->getTotalNumFrames()&&video->getTotalNumFrames()!=0&&bLoading==true){
                for(int i=0;i<numFrames;i++){
                    video->setFrame(i);
                    pixels.push_back(video->getPixels());
                }
                bFirst=false;
                bFinished=true;
                bLoaded=true;
                unlock();
            }
            else if (bLoading==true){
                if(video->getTotalNumFrames()>0){
                    numFrames=video->getTotalNumFrames();
                }
            }
        }
    }
    
    void start(){
        startThread(true, true);
        video=new ofQTKitPlayer();
    }
    
    void stop(){
        stopThread();
    }
    

};

