//
//  VideoThread.h
//  osxHighPerformanceVideoPlayerExample
//
//  Created by IncredibleMachines on 2/26/14.
//
//

#pragma once

#include "ofMain.h"

class VideoThread : public ofThread{
public:
    
    ofVideoPlayer * video;
    string filepath;
    bool bRun;
    bool bLoading;
    bool bFinished;
    ofQTKitDecodeMode decodeMode;
    float timer;
    
    
    VideoThread(){
        bRun=false;
        bLoading=false;
        bFinished=false;
        timer=0;
        video = new ofVideoPlayer();
        video->setPixelFormat(OF_PIXELS_RGB);
        video->setUseTexture(false);
        cout<<"created"<<endl;
    };
    
    void threadedFunction(){
        while(isThreadRunning() !=0){
            if(bRun==true){
                if(lock()){
                    cout<<"go"<<endl;
                    cout<<filepath<<endl;
                    video->loadMovie(filepath);
                    bRun=false;
                    bLoading=true;
                    timer=ofGetElapsedTimeMillis();
                    cout<<timer<<endl;
                }
            }
            if(ofGetElapsedTimeMillis()-timer>2000&&bLoading==true){
                    cout<<"restart"<<endl;
                    bLoading=false;
                    bFinished=true;
                    unlock();
            }
        }
    }
    
    void start(){
        startThread(true,true);

    }
    
    void stop(){
//        video->stop();
//        video->close();
//        delete video;
        stopThread();
    }
    
    
};
