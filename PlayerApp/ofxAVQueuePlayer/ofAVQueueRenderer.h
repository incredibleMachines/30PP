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

#define BUFFER_SIZE 3

@interface AVQueueRenderer : AVPlayer <AVPlayerItemOutputPullDelegate> {
    AVQueuePlayer * _queuePlayer;
    AVPlayerItem * _playerItem;
    AVPlayerItem *_loadItem;
    AVPlayerItem *_loadItem1;
    AVPlayerItem *_loadItem2;
    dispatch_queue_t _outputQ;
    
    BOOL _bLoading;
    BOOL _bLoaded;
    BOOL _bPaused;
    BOOL _bFinished;
    BOOL _bLoops;
    CMTime _currentTime;
    CMTime _duration;
    CMTime _startTime;
    double _playbackRate;
    double _frameRate;
    
    CGSize _videoSize;
    
    int index;
    
    id _playerItemVideoOutput;
    CVPixelBufferRef _latestPixelFrame;
    
    CVDisplayLinkRef displayLink;
    
    
}

@property (nonatomic, retain) AVQueuePlayer * qPlayer;

@property (nonatomic, assign, readonly) double width;
@property (nonatomic, assign, readonly) double height;

@property (nonatomic, assign, readonly, getter = isLoading) BOOL bLoading;
@property (nonatomic, assign, readonly) BOOL bLoaded;
@property (nonatomic, assign, readonly) BOOL bFinished;

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

- (void)loadURL:(NSURL *)url;

- (void)addVideo:(NSString *)filePath;
- (void)advanceVideo;

- (void)play;

- (BOOL)update;
- (void)pixels:(unsigned char *)outbuf;

- (void)initArray:(NSArray*)files;
- (void)initPlayer:(NSString*)string;
- (void)makeActive;

@end
