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
    
     VideoThread();
    
    void threadedFunction();
    
    void start();
    
    void stop();
    
    ofVideoPlayer video;
    string filepath;
    bool bRun;
    bool bLoading;
    bool bFinished;
    bool bDone;
    ofQTKitDecodeMode decodeMode;
    float timer;
    
    
    
};
