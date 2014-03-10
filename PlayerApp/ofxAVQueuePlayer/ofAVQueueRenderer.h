//
//  ofAVQueuePlayer.h
//  openFrameworksLib
//
//  Created by IncredibleMachines on 2/28/14.
//
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import <AVFoundation/AVFoundation.h>
#import <OpenGL/OpenGL.h>

#define FREEWHEELING_PERIOD_IN_SECONDS 0.1
#define ADVANCE_INTERVAL_IN_SECONDS 0.03

@interface AVQueueRenderer : AVPlayer <AVPlayerItemOutputPullDelegate> {
    AVQueuePlayer * _queuePlayer;
    AVPlayerItem * _playerItem;
    AVPlayerItem *_loadItem;
    
    AVPlayerItemVideoOutput *_playerItemVideoOutput;
	CVDisplayLinkRef _displayLink;
	CMVideoFormatDescriptionRef _videoInfo;
	
	uint64_t _lastHostTime;
	dispatch_queue_t _queue;
    
    BOOL _useTexture;
    BOOL _useAlpha;
    
    BOOL _bLoading;
    BOOL _bLoaded;
    BOOL _bPaused;
    BOOL _bFinished;
    BOOL _bLoops;
    
    CGSize _videoSize;
    CMTime _currentTime;
    CMTime _duration;
    CMTime _startTime;
    double _playbackRate;
    double _frameRate;
    
    BOOL _newFrame;
    
//    id _playerItemVideoOutput;
    CVOpenGLTextureCacheRef _textureCache;
	CVOpenGLTextureRef _latestTextureFrame;
    CVPixelBufferRef _latestPixelFrame;
    
    int _pixelLoadCount;
    
    unsigned char * _pix;
    
    
}

@property (nonatomic, retain) AVQueuePlayer * qPlayer;

@property (nonatomic, assign, readonly) double width;
@property (nonatomic, assign, readonly) double height;

@property (nonatomic, assign, readonly, getter = isLoading) BOOL bLoading;
@property (nonatomic, assign, readonly) BOOL bLoaded;
@property (nonatomic, assign, readonly) BOOL bFinished;
@property (nonatomic, assign, readonly) BOOL bNewFrame;

@property (nonatomic, assign) BOOL useAlpha;
@property (nonatomic, assign) BOOL useTexture;

@property (nonatomic, assign, getter = isPaused, setter = setPaused:) BOOL bPaused;
//@property (nonatomic, assign, readonly) BOOL isPlaying;

@property (nonatomic, assign, readonly) double frameRate;
@property (nonatomic, assign, readonly) double duration;
@property (nonatomic, assign, readonly) int totalFrames;
@property (nonatomic, assign) double currentTime;
@property (nonatomic, assign) int currentFrame;
@property (nonatomic, assign) double position;
@property (nonatomic, assign) double playbackRate;
@property (nonatomic, assign, getter = loops, setter = setLoops:) BOOL bLoops;
        
@property (nonatomic, assign, readonly) BOOL textureAllocated;
@property (nonatomic, assign, readonly) GLuint textureID;
@property (nonatomic, assign, readonly) GLenum textureTarget;

@property (nonatomic, assign) unsigned char * pix;

- (void)loadURL:(NSURL *)url;

- (void)addVideo:(NSString *)filePath;
- (void)advanceVideo;

- (void)play;

- (BOOL)update;
- (void)bindTexture;
- (void)unbindTexture;
- (void)pixels:(unsigned char *)outbuf;

- (void)initPlayer:(NSString*)string;
- (void)makeActive;

@end
