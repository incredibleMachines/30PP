//
//  ofAVQueuePlayer.cpp
//  openFrameworksLib
//
//  Created by IncredibleMachines on 2/28/14.
//
//

#include "ofAVQueuePlayer.h"
#include "Poco/String.h"

//--------------------------------------------------------------
ofAVQueuePlayer::ofAVQueuePlayer()
{
    moviePlayer = NULL;
	bNewFrame = false;
    bPaused = true;
	duration = 0.0f;
    speed = 1.0f;
	
    scrubToTime = 0.0;
    bInitialized = false;
    
    pixelFormat = OF_PIXELS_RGB;
    currentLoopState = OF_LOOP_NORMAL;
    
    NSLog(@"player constructor");
    
}

//--------------------------------------------------------------
ofAVQueuePlayer::~ofAVQueuePlayer()
{
	close();
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::addVideo(string path)
{
    
//    if (bInitialized) {
//        close();
//    }
	
	@autoreleasepool{
            path = ofToDataPath(path, false);
            [moviePlayer addVideo:[NSString stringWithUTF8String:path.c_str()]];
    }
    return true;
}
//--------------------------------------------------------------
void ofAVQueuePlayer::close()
{
    pixels.clear();
	
    if (moviePlayer != nil) {
        @autoreleasepool {
            [moviePlayer release];
            moviePlayer = nil;
        }
    }
    
    bInitialized = false;
    bNewFrame = false;
}

//--------------------------------------------------------------
void ofAVQueuePlayer::idleMovie()
{
    update();
}

//--------------------------------------------------------------
void ofAVQueuePlayer::update()
{
    if (!moviePlayer) {
        NSLog(@"no moviePlayer");
        return;
    }
    
    if ([moviePlayer bLoaded]) {
//        NSLog(@"update");
        if (!bInitialized) {
			reallocatePixels();
            bInitialized = true;
            
            if (scrubToTime != 0.0f) {
				setTime(scrubToTime);
				scrubToTime = 0.0f;
			}
            
			if (bShouldPlay){
				play();
				bShouldPlay = false;
			}
        }
		bNewFrame = [moviePlayer update];
        bHavePixelsChanged = bNewFrame;
    }
    else {
        ofLogNotice("ofAVQueuePlayer::update()") << "Movie player not ready";
    }
}

//--------------------------------------------------------------
void ofAVQueuePlayer::play()
{
	if (bInitialized) {
        NSLog(@"bInit PLAY");
        ofLogVerbose("ofAVQueuePlayer::play()") << "Initialized and playing at time " << getCurrentTime();
		[moviePlayer play];
	}
	else {
		bShouldPlay = true;
	}
}


//--------------------------------------------------------------
bool ofAVQueuePlayer::isFrameNew()
{
    return bNewFrame;
}

//--------------------------------------------------------------
unsigned char * ofAVQueuePlayer::getPixels()
{
	return getPixelsRef().getPixels();
}

//--------------------------------------------------------------
ofPixelsRef ofAVQueuePlayer::getPixelsRef()
{
	if (isLoaded()) {
		// Don't get the pixels every frame if it hasn't updated
//		if (bHavePixelsChanged) {
//			[moviePlayer pixels:pixels.getPixels()];
//			bHavePixelsChanged = false;
//		}
        if (bHavePixelsChanged) {
            if(moviePlayer.pix){
                cout<<"getPixels width"<<[moviePlayer width]<<endl;
                pixels.setFromExternalPixels(moviePlayer.pix, [moviePlayer width],[moviePlayer height], 3);
            }
            bHavePixelsChanged=false;
        }
	}
	else {
		ofLogError("ofAVQueuePlayer::getPixelsRef()") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixelsRef.";
	}
	return pixels;
}

//--------------------------------------------------------------
ofTexture* ofAVQueuePlayer::getTexture()
{
	//TODO: Allow AVF's direct to texture
	if (moviePlayer.textureAllocated) {
		updateTexture();
	}
	return &tex;
    
    //    return NULL;
}

//--------------------------------------------------------------
ofTexture& ofAVQueuePlayer::getTextureReference()
{
    //	getTexture();
	updateTexture();
	return tex;
}

//--------------------------------------------------------------
void ofAVQueuePlayer::updateTexture()
{
    if(!moviePlayer.textureAllocated) {
		return;
	}
	
	tex.setUseExternalTextureID(moviePlayer.textureID);
	ofTextureData& data = tex.getTextureData();
	data.textureTarget = moviePlayer.textureTarget;
	data.width = getWidth();
	data.height = getHeight();
	data.tex_w = getWidth();
	data.tex_h = getHeight();
	data.tex_t = getWidth();
	data.tex_u = getHeight();
    
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::isLoading()
{
    return moviePlayer && [moviePlayer isLoading];
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::isLoaded()
{
    return bInitialized;
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::isPlaying()
{
    return moviePlayer && [moviePlayer isPlaying];
}


//--------------------------------------------------------------
float ofAVQueuePlayer::getPosition()
{
    return moviePlayer.position;
}

//--------------------------------------------------------------
float ofAVQueuePlayer::getCurrentTime()
{
    return moviePlayer.currentTime;
}

//--------------------------------------------------------------
float ofAVQueuePlayer::getPositionInSeconds()
{
    return getCurrentTime();
}

//--------------------------------------------------------------
int ofAVQueuePlayer::getCurrentFrame()
{
    return moviePlayer.currentFrame;
}

//--------------------------------------------------------------
float ofAVQueuePlayer::getDuration()
{
    return moviePlayer.duration;
}

//--------------------------------------------------------------
int ofAVQueuePlayer::getTotalNumFrames()
{
    return moviePlayer.totalFrames;
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::isPaused()
{
    return moviePlayer && [moviePlayer isPaused];
}

//--------------------------------------------------------------
float ofAVQueuePlayer::getSpeed()
{
    if (moviePlayer) {
        return moviePlayer.playbackRate;
    }
    
    return 0;
}

//--------------------------------------------------------------
ofLoopType ofAVQueuePlayer::getLoopState()
{
    if (moviePlayer && [moviePlayer loops])
        return OF_LOOP_NORMAL;
    
	return OF_LOOP_NONE;
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setPosition(float pct)
{
    [moviePlayer setPosition:pct];
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setTime(float position)
{
	if (![moviePlayer isLoaded]) {
		ofLogNotice("ofAVQueuePlayer::setCurrentTime()") << "Video player not ready, declaring to scrub to time " << scrubToTime;
		scrubToTime = position;
	}
	else {
        [moviePlayer setCurrentTime:position];
	}
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setPositionInSeconds(float seconds)
{
    setTime(seconds);
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setFrame(int frame)
{
    [moviePlayer setCurrentFrame:frame];
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setVolume(float volume)
{
    [moviePlayer setVolume:volume];
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setPaused(bool bPaused)
{
    [moviePlayer setPaused:bPaused];
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setLoopState(ofLoopType state)
{
    if (moviePlayer) {
        [moviePlayer setLoops:(state == OF_LOOP_NORMAL)];
    }
    
    if (state == OF_LOOP_PALINDROME) {
        ofLogWarning("ofAVQueuePlayer::setLoopState") << "No palindrome yet, sorry!";
    }
}

//--------------------------------------------------------------
void ofAVQueuePlayer::setSpeed(float speed)
{
    [moviePlayer setPlaybackRate:speed];
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::setPixelFormat(ofPixelFormat newPixelFormat)
{
    if (newPixelFormat != OF_PIXELS_RGB && newPixelFormat != OF_PIXELS_RGBA) {
        ofLogWarning("ofAVQueuePlayer::setPixelFormat") << "Pixel format " << newPixelFormat << " is not supported.";
        return false;
    }
    
//    if (newPixelFormat != pixelFormat) {
//        pixelFormat = newPixelFormat;
//        // If we already have a movie loaded we need to reload
//        // the movie with the new settings correctly allocated.
//        if (isLoaded()) {
//            loadMovie(moviePath);
//        }
//    }
	return true;
}

//--------------------------------------------------------------
ofPixelFormat ofAVQueuePlayer::getPixelFormat()
{
    return pixelFormat;
}

//--------------------------------------------------------------
void ofAVQueuePlayer::draw(float x, float y)
{
    if (!bInitialized) return;
    
	draw(x, y, getWidth(), getHeight());
}

//--------------------------------------------------------------
void ofAVQueuePlayer::draw(float x, float y, float w, float h)
{
    if (!bInitialized) return;
    
	tex.draw(x, y, w, h);
}

//--------------------------------------------------------------
float ofAVQueuePlayer::getWidth()
{
//    NSLog(@"width %f",moviePlayer.width);
    return moviePlayer.width;
}

//--------------------------------------------------------------
float ofAVQueuePlayer::getHeight()
{
    return moviePlayer.height;
}

//--------------------------------------------------------------
void ofAVQueuePlayer::firstFrame()
{
    //TODO:
}

//--------------------------------------------------------------
void ofAVQueuePlayer::nextFrame()
{
    //TODO:
}

//--------------------------------------------------------------
void ofAVQueuePlayer::previousFrame()
{
	//TODO:
}


//--------------------------------------------------------------
void ofAVQueuePlayer::reallocatePixels()
{
    if (pixelFormat == OF_PIXELS_RGBA) {
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR_ALPHA);
    }
    else {
                    cout<<"allocated width"<<getWidth()<<endl;
        cout<<"allocated height"<<getHeight()<<endl;
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR);
    }
}

//--------------------------------------------------------------
void ofAVQueuePlayer::nextVideo(){
    NSLog(@"play next");
    [moviePlayer advanceVideo];
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::getLoadedState(){
    return [moviePlayer bLoaded];
}

//--------------------------------------------------------------
bool ofAVQueuePlayer::getFinished(){
    return [moviePlayer bFinished];
}

//--------------------------------------------------------------
void ofAVQueuePlayer::initPlayer(string filepath){
    
    @autoreleasepool{
        NSLog(@"playerInit");

            filepath = ofToDataPath(filepath, false);
        
        NSString * string=[NSString stringWithUTF8String:filepath.c_str()];
        moviePlayer=[AVQueueRenderer alloc];
        [moviePlayer initPlayer:string];
    }
}