//
//  VideoThread.cpp
//  emptyExample
//
//  Created by IncredibleMachines on 2/26/14.
//
//

#include "VideoThread.h"

VideoThread::VideoThread(){
    bRun=false;
    bLoading=false;
    bFinished=false;
    bDone=true;
    timer=0;
    
    //        video = new ofVideoPlayer();



    cout<<"created"<<endl;
}

void VideoThread::threadedFunction(){
    while(isThreadRunning() !=0){
        if(bRun==true){
            if(lock()){
                cout<<"go"<<endl;
                cout<<filepath<<endl;
                video.stop();
                video.close();
                video.setPixelFormat(OF_PIXELS_RGB);
                video.setUseTexture(false);
                video.loadMovie(filepath);
                bRun=false;
                bLoading=true;
                timer=ofGetElapsedTimeMillis();
                bDone=false;
            }

            
        }
        
        else if(ofGetElapsedTimeMillis()-timer>2000&&bLoading==true){
            cout<<"restart"<<endl;
            bLoading=false;
            bFinished=true;
            unlock();
        }

        
    }

}

void VideoThread::start(){
    startThread(true,true);
    video.setPixelFormat(OF_PIXELS_RGB);
    video.setUseTexture(false);
    
    
}

void VideoThread::stop()
{
    video.stop();
    video.close();
    //        delete video;
    stopThread();
}