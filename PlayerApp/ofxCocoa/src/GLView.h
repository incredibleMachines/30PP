/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

/***************
 DEPENDENCIES:
 - NONE
 ***************/ 

#import "ofConstants.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <Quartz/Quartz.h>
#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>
//#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>
#import "AppWindow.h"

void ofGLReadyCallback();

@interface GLView : NSOpenGLView {
    
    /***** 30PP *****/
    AVPlayer * _player;
    AVPlayerItem * _playerItem;
    AVPlayerItemVideoOutput * _playerItemVideoOutput;
    unsigned char * _pixels;
    BOOL _bLoaded;
    BOOL _bPaused;
    BOOL _bFinished;
    
    CMTime _currentTime;
    CMTime _duration;
    CMTime _startTime;
    double _playbackRate;
    double _frameRate;
    
    CGSize _videoSize;
    
    uint64_t _lastHostTime;
    int _texID;
    int _glFormat;
    int _glType;
    BOOL _bPixelsAllocated;
    
    unsigned char * _pix;
    
    CVOpenGLTextureCacheRef _textureCache;
	CVOpenGLTextureRef _latestTextureFrame;
    CVPixelBufferPoolRef _pixelBuffer;
    
    /****************/
    
	NSRect			savedWindowFrame;
	int				windowMode;
	
	NSOpenGLContext *openGLContext;
	NSOpenGLPixelFormat *pixelFormat;
	
	float			targetFrameRate;
	BOOL			useDisplayLink;
	CVDisplayLinkRef displayLink;
	NSTimer			*timer;
	BOOL			isAnimating;
    
	// added BR: capture external mouse events
	BOOL			captureExternalMouseEvents;
	BOOL			externalMouseEventsActive;
	id mouseMoveHandler, leftMouseDownHandler, rightMouseDownHandler, leftMouseUpHandler, rightMouseUpHandler, leftMouseDraggedHandler,rightMouseDraggedHandler;
    
    CFMutableDictionaryRef dict;
    
    

}

    /***** 30PP *****/
- (NSDictionary *)pixelBufferAttributes;
@property (nonatomic, retain)  AVPlayer * player;
@property (nonatomic, retain)  AVPlayerItem * playerItem;
@property (nonatomic, retain) AVPlayerItemVideoOutput * playerItemVideoOutput;

@property (nonatomic, assign, readonly) double width;
@property (nonatomic, assign, readonly) double height;

@property (nonatomic, assign, readonly) BOOL bLoaded;
@property (nonatomic, assign, readonly) BOOL bFinished;

@property (nonatomic, assign, getter = isPaused, setter = setPaused:) BOOL bPaused;
//@property (nonatomic, assign, readonly) BOOL isPlaying;

@property (nonatomic, assign, readonly) double videoFrameRate;
@property (nonatomic, assign, readonly) double duration;
@property (nonatomic, assign, readonly) int totalFrames;
@property (nonatomic, assign) double currentTime;
@property (nonatomic, assign) int currentFrame;
@property (nonatomic, assign) double position;
@property (nonatomic, assign) double playbackRate;

@property (nonatomic, assign, readonly) GLuint textureID;

    /***** 30PP *****/


@property (readonly) BOOL useDisplayLink;
@property (readonly) int windowMode;
@property (readonly) NSOpenGLContext* openGLContext;
@property (readonly) NSOpenGLPixelFormat* pixelFormat;
@property (readonly, getter=getCaptureExternalMouseEvents) BOOL captureExternalMouseEvents;


-(id) initWithFrame:(NSRect)frameRect;
-(id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context;

-(void) setSyncToDisplayLink:(BOOL)b;

-(void) updateAndDraw;
-(void) startAnimation;
-(void) stopAnimation;
-(void) toggleAnimation;

-(void) setFrameRate:(float)rate;


-(void)goFullscreen:(NSScreen*)screen;
-(void)goWindow;
-(void)toggleFullscreen;

// BR
-(void) setCaptureExternalMouseEvents:(BOOL)b;

/***** 30PP *****/
-(void) initPlayer:(NSString*)path ID:(int)ID;
/***** 30PP *****/



@end
