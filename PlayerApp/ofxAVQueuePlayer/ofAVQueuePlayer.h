//
//  ofAVQueuePlayer.h
//  openFrameworksLib
//
//  Created by IncredibleMachines on 2/28/14.
//
//

#pragma once

#include "ofMain.h"

#ifdef __OBJC__
#import "ofAVQueueRenderer.h"
#endif


class ofAVQueuePlayer  {
    
public:
    
    ofAVQueuePlayer();
    ~ofAVQueuePlayer();
    
    bool                addVideo(string path);
    void                initPlayer(string filepath);
    void                nextVideo();
    
    void                close();
    
    void                idleMovie();
    void                update();
    void                play();
	
    bool                isFrameNew(); //returns true if the frame has changed in this update cycle
    
    // Returns openFrameworks compatible RGBA pixels.
    // Be aware of your current render mode.
    unsigned char *     getPixels();
    ofPixelsRef         getPixelsRef();
    
    bool                isLoading();
    bool                isLoaded();
    
    bool                isPlaying();
    bool                getFinished();
    
    float               getPosition();
    float               getPositionInSeconds();
    float               getCurrentTime();
    int                 getCurrentFrame();
    int                 getTotalNumFrames();
    float               getDuration();
    bool                isPaused();
    float               getSpeed();
    ofLoopType          getLoopState();
    
    ofTexture *         getTexture();
    ofTexture&          getTextureReference();
    
    void                setPosition(float pct);
	void                setTime(float seconds);
    void                setPositionInSeconds(float seconds);
    void                setFrame(int frame); // frame 0 = first frame...
    void                setPaused(bool bPaused);
    void                setSpeed(float speed);
    void                setLoopState(ofLoopType state);
    void                setVolume(float volume);
    
    bool                 getLoadedState();
    
    // ofAVQueuePlayer only supports OF_PIXELS_RGB and OF_PIXELS_RGBA.
    bool                setPixelFormat(ofPixelFormat pixelFormat);
    ofPixelFormat       getPixelFormat();
    
    void                draw(float x, float y, float w, float h);
    void                draw(float x, float y);
    
    float               getWidth();
    float               getHeight();
    
    void                firstFrame();
    void                nextFrame();
    void                previousFrame();
    

    
protected:
    
    ofLoopType currentLoopState;
    
    //bool bTheFutureIsNow;
    
    bool bPaused;
	bool bShouldPlay;
	
	float scrubToTime;
    bool bNewFrame;
    bool bHavePixelsChanged;
    
    float duration;
    float speed;
    
    string moviePath;
    
    bool bInitialized;
    
    void reallocatePixels();
    void updateTexture();
    
    //    ofFbo fbo;
    ofTexture tex;
    ofPixels pixels;
    ofPixelFormat pixelFormat;
    
    // This #ifdef is so you can include this .h file in .cpp files
    // and avoid ugly casts in the .m file
#ifdef __OBJC__
    AVQueueRenderer* moviePlayer;
    AVQueueRenderer* moviePlayer2;
#else
    void * moviePlayer;
#endif
    
};