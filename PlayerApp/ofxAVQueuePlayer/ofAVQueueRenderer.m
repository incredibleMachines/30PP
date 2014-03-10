//
//  ofAVQueuePlayer.m
//  openFrameworksLib
//
//  Created by IncredibleMachines on 2/28/14.
//
//  based onofxAVFVideoRenderer.m
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//
//

#import "ofAVQueueRenderer.h"
#import <Accelerate/Accelerate.h>

@interface AVQueueRenderer ()

- (NSDictionary *)pixelBufferAttributes;

@property (nonatomic, retain)  AVPlayerItem * playerItem;
@property (nonatomic, retain)  AVPlayerItem * loadItem;
@property (nonatomic, retain) AVPlayerItemVideoOutput * playerItemVideoOutput;


@end

@implementation AVQueueRenderer

@synthesize qPlayer=_queuePlayer;
@synthesize playerItem=_playerItem;
@synthesize loadItem=_loadItem;
@synthesize playerItemVideoOutput = _playerItemVideoOutput;

@synthesize useTexture = _useTexture;
@synthesize useAlpha = _useAlpha;

@synthesize bLoading = _bLoading;
@synthesize bLoaded = _bLoaded;
@synthesize bPaused = _bPaused;
@synthesize bFinished=_bFinished;
@synthesize bNewFrame=_newFrame;


@synthesize frameRate = _frameRate;
@synthesize playbackRate = _playbackRate;
@synthesize bLoops = _bLoops;
@synthesize pix=_pix;

static CVReturn displayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext);

- (id)init{
    self=[super init];
    NSLog(@"init renderer");
    return self;
}

- (NSDictionary *)pixelBufferAttributes
{
    // kCVPixelFormatType_32ARGB, kCVPixelFormatType_32BGRA, kCVPixelFormatType_422YpCbCr8
    return @{
             (NSString *)kCVPixelBufferOpenGLCompatibilityKey : [NSNumber numberWithBool:NO],
             (NSString *)kCVPixelBufferPixelFormatTypeKey     : [NSNumber numberWithInt:kCVPixelFormatType_32ARGB]
			 //[NSNumber numberWithInt:kCVPixelFormatType_422YpCbCr8]
             };
}

- (void)addVideo:(NSString*)filePath{
    [self loadURL:[NSURL fileURLWithPath:[filePath stringByStandardizingPath]]];
    int qSize=[[self.qPlayer items] count];
    NSLog(@"ADD VIDEO array size %i",qSize);
}

- (void)loadURL:(NSURL *)url{
    
//    _bLoading = YES;
//    _bLoaded = NO;
    _bPaused = NO;
    
    NSLog(@"Loading %@", [url absoluteString]);
    
    AVURLAsset *asset = [AVURLAsset URLAssetWithURL:url options:nil];
    NSString *tracksKey = @"tracks";
    
    [asset loadValuesAsynchronouslyForKeys:@[tracksKey] completionHandler: ^{
        static const NSString *kItemStatusContext;
        
        // Perform the following back on the main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            
            // Check to see if the file loaded
            NSError *error;
            AVKeyValueStatus status = [asset statusOfValueForKey:tracksKey error:&error];
            
            if (status == AVKeyValueStatusLoaded) {
                
                
//                AVAssetTrack *videoTrack = [[asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
//                _videoSize = [videoTrack naturalSize];
//                _currentTime = kCMTimeZero;
//                _duration = CMTimeAdd(_duration,asset.duration);
//                _frameRate = [videoTrack nominalFrameRate];
                
                self.playerItem = [AVPlayerItem playerItemWithAsset:asset];
                
                    //insert into queue
                if([_queuePlayer canInsertItem:self.playerItem afterItem:nil]){
                    NSLog(@"can insert items");
                    [_queuePlayer insertItem:self.playerItem afterItem:nil];
                }
                int qSize=[[_queuePlayer items] count];
                NSLog(@"array size %i",qSize);
            
//                _bLoading = NO;
//                _bLoaded = YES;
                NSLog(@"async loaded");
            }
    
        });
    }];
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    //	// There is no autorelease pool when this method is called
    //	// because it will be called from a background thread.
    //	// It's important to create one or app can leak objects.
    //	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    ////	
    ////	[self drawView];
    //	
    //	[pool release];
	return kCVReturnSuccess;
}


-(void)initPlayer:(NSString *)string{
    
    NSLog(@"init");
        _bLoading = NO;
        _bLoaded = NO;
        _bPaused = NO;
        _bFinished = NO;
        
        _frameRate = 0.0;
        _playbackRate = 1.0;
        _bLoops = false;
    
        _useTexture = NO;
        _useAlpha = NO;
    _newFrame=NO;
    
    _queue = dispatch_queue_create(NULL, NULL);
    
    _playerItemVideoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:@{(id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32ARGB)}];
    if (_playerItemVideoOutput)
    {
        // Create a CVDisplayLink to receive a callback at every vsync
        CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
        CVDisplayLinkSetOutputCallback(_displayLink, displayLinkCallback, (__bridge void *)self);
        // Pause the displayLink till ready to conserve power
        CVDisplayLinkStop(_displayLink);
        
//        	CVDisplayLinkStart(_displayLink);
        
        // Request notification for media change in advance to start up displayLink or any setup necessary
        [_playerItemVideoOutput setDelegate:self queue:_queue];
        [_playerItemVideoOutput requestNotificationOfMediaDataChangeWithAdvanceInterval:ADVANCE_INTERVAL_IN_SECONDS];
    }
    
//    // Create and attach video output. 10.8 Only!!!
//    self.playerItemVideoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:[self pixelBufferAttributes]];
//    [self.playerItemVideoOutput autorelease];
//    if (self.playerItemVideoOutput) {
//        [(AVPlayerItemVideoOutput *)self.playerItemVideoOutput setSuppressesPlayerRendering:YES];
//    }
//    _outputQ = dispatch_queue_create("outputQ", DISPATCH_QUEUE_PRIORITY_BACKGROUND);
//    [[self playerItemVideoOutput] setDelegate:self queue:_outputQ];
//     [self.playerItemVideoOutput requestNotificationOfMediaDataChangeWithAdvanceInterval:0.03];
    
    NSURL *url=[NSURL fileURLWithPath:[string stringByStandardizingPath]];
    
    
    NSLog(@"Loading %@", [url absoluteString]);

    
    AVURLAsset *asset = [AVURLAsset URLAssetWithURL:url options:nil];
    NSString *tracksKey = @"tracks";
    
    [asset loadValuesAsynchronouslyForKeys:@[tracksKey] completionHandler: ^{
            static const NSString *kItemStatusContext;
            // Perform the following back on the main thread
            dispatch_async(dispatch_get_main_queue(), ^{
                
                // Check to see if the file loaded
                NSError *error;
                AVKeyValueStatus status = [asset statusOfValueForKey:tracksKey error:&error];
                
                if (status == AVKeyValueStatusLoaded) {
                    
                    
                    AVAssetTrack *videoTrack = [[asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
                    _videoSize = [videoTrack naturalSize];
                    _currentTime = kCMTimeZero;
                    _duration = asset.duration;
                    _frameRate = [videoTrack nominalFrameRate];
                    
                    
                    
                    self.loadItem = [AVPlayerItem playerItemWithAsset:asset];
                    
                    NSLog(@"create player");
                    
                    NSArray * loadArray=[NSArray arrayWithObjects:self.loadItem, nil];
                    
                    
                    self.qPlayer=[[AVQueuePlayer alloc] initWithItems:loadArray];
                    NSLog(@"PLAYER CREATED");
                    
                    NSLog(@"width: %lf",[self width]);
                    NSLog(@"height: %lf",[self height]);
                    int size=[self width] * [self height] *3;
                    NSLog(@"char array size: %i", size);
                    
                    _pix= (unsigned char*) calloc(size, sizeof(unsigned char));
                    
                    NSLog(@"char array init: %lu", sizeof _pix);
                    
//                  [self.playerItem addObserver:self forKeyPath:@"status" options:0 context:&kItemStatusContext];
                    
                    
                    // Notify this object when the player reaches the end
                    // This allows us to loop the video
                    [[NSNotificationCenter defaultCenter]
                     addObserver:self
                     selector:@selector(playerItemDidReachEnd:)
                     name:AVPlayerItemDidPlayToEndTimeNotification
                     object:[self.qPlayer currentItem]];
                    
                    [[self.qPlayer currentItem] addOutput:self.playerItemVideoOutput];
                    
//                  CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
//                                            
//                  // Set the renderer output callback function
//                  CVDisplayLinkSetOutputCallback(displayLink, &DisplayLinkCallback, self);
//
//                                            
//                                            // Activate the display link
//              CVDisplayLinkStart(displayLink);
                    
                // Create CVOpenGLTextureCacheRef for optimal CVPixelBufferRef to GL texture conversion.
                if (self.useTexture && !_textureCache) {
                    CVReturn err = CVOpenGLTextureCacheCreate(kCFAllocatorDefault, NULL, CGLGetCurrentContext(), CGLGetPixelFormat(CGLGetCurrentContext()), NULL, &_textureCache);
                    if (err != noErr) {
                        NSLog(@"Error at CVOpenGLTextureCacheCreate %d", err);
                    }
                }
                }
                _bLoaded=YES;
                _bLoading=NO;
            });
        }];

}


-(void) advanceVideo{

    
    [self.qPlayer advanceToNextItem];
    NSLog(@"advanced");
    int qSize=[[_queuePlayer items] count];
    NSLog(@"array size %i",qSize);
    _currentTime = kCMTimeZero;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[self.qPlayer currentItem] addOutput:self.playerItemVideoOutput];
    });
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(playerItemDidReachEnd:)
     name:AVPlayerItemDidPlayToEndTimeNotification
     object:[self.qPlayer currentItem]];
    
//    [self.qPlayer seekToTime:kCMTimeZero];
    [self.qPlayer pause];
}

- (void)play
{
    [self.qPlayer play];
        int qSize=[[_queuePlayer items] count];
    NSLog(@"array size %i",qSize);
}

- (void)setPaused:(BOOL)bPaused
{
    _bPaused = bPaused;
    if (_bPaused) {
        [self.qPlayer pause];
        [self.qPlayer seekToTime:kCMTimeZero];
    }
    else {
        [self.qPlayer play];
        self.qPlayer.rate = _playbackRate;
    }
}

static CVReturn displayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext)
{
	AVQueueRenderer *self = (__bridge AVQueueRenderer *)displayLinkContext;
	AVPlayerItemVideoOutput *playerItemVideoOutput = self->_playerItemVideoOutput;
	
	// The displayLink calls back at every vsync (screen refresh)
	// Compute itemTime for the next vsync
	CMTime outputItemTime = [playerItemVideoOutput itemTimeForCVTimeStamp:*inOutputTime];
	if ([playerItemVideoOutput hasNewPixelBufferForItemTime:outputItemTime])
	{
		self->_lastHostTime = inOutputTime->hostTime;
		
		// Copy the pixel buffer to be displayed next and add it to AVSampleBufferDisplayLayer for display
		CVPixelBufferRef pixBuff = [playerItemVideoOutput copyPixelBufferForItemTime:outputItemTime itemTimeForDisplay:NULL];
        
//        self->_latestPixelFrame=[playerItemVideoOutput copyPixelBufferForItemTime:outputItemTime itemTimeForDisplay:NULL];
//		
        if ((NSInteger)self.width != CVPixelBufferGetWidth(pixBuff) || (NSInteger)self.height != CVPixelBufferGetHeight(pixBuff)) {
            NSLog(@"CoreVideo pixel buffer is %ld x %ld while self reports size of %ld x %ld. This is most likely caused by a non-square pixel video format such as HDV. Open this video in texture only mode to view it at the appropriate size",
                  CVPixelBufferGetWidth(pixBuff), CVPixelBufferGetHeight(pixBuff), (long)self.width, (long)self.height);
            return;
        }
        
        if (CVPixelBufferGetPixelFormatType(pixBuff) != kCVPixelFormatType_32ARGB) {
            NSLog(@"QTKitMovieRenderer - Frame pixelformat not kCVPixelFormatType_32ARGB: %d, instead %ld", kCVPixelFormatType_32ARGB, (long)CVPixelBufferGetPixelFormatType(pixBuff));
            return;
        }
        
        CVPixelBufferLockBaseAddress(pixBuff, kCVPixelBufferLock_ReadOnly);
        //If we are using alpha, the ofxAVFVideoPlayer class will have allocated a buffer of size
        //video.width * video.height * 4
        //CoreVideo creates alpha video in the format ARGB, and openFrameworks expects RGBA,
        //so we need to swap the alpha around using a vImage permutation
        
//        NSLog(@"buffer width calc: %zu", CVPixelBufferGetWidth(pixBuff)*3);
//        NSLog(@"buffer width report: %zu", CVPixelBufferGetBytesPerRow(pixBuff));
//        NSLog(@"buffer height: %zu", CVPixelBufferGetHeight(pixBuff));
//        NSLog(@"buffer array size: %zu", CVPixelBufferGetHeight(pixBuff)*CVPixelBufferGetBytesPerRow(pixBuff));
        
        vImage_Buffer src = {
            CVPixelBufferGetBaseAddress(pixBuff),
            CVPixelBufferGetHeight(pixBuff),
            CVPixelBufferGetWidth(pixBuff),
            CVPixelBufferGetBytesPerRow(pixBuff)
        };
        vImage_Error err;
        //If we are are doing RGB then ofxAVFVideoPlayer will have created a buffer of size video.width * video.height * 3
        //so we use vImage to copy them into the out buffer
        
//        NSLog(@"copying buffer");
        vImage_Buffer dest = { self->_pix, self.height, self.width, 3*self.width};
        err = vImageConvert_ARGB8888toRGB888(&src, &dest, 0);
//        NSLog(@"copied");
        
        CVPixelBufferUnlockBaseAddress(pixBuff, kCVPixelBufferLock_ReadOnly);
        
        if (err != kvImageNoError) {
            NSLog(@"Error in Pixel Copy vImage_error %ld", err);
        }
        
        self->_newFrame=YES;
//
		CVBufferRelease(pixBuff);
	}
	else
	{
        self->_newFrame=NO;
		CMTime elapsedTime = CMClockMakeHostTimeFromSystemUnits(inNow->hostTime - self->_lastHostTime);
		if (CMTimeGetSeconds(elapsedTime) > FREEWHEELING_PERIOD_IN_SECONDS)
		{
			// No new images for a while.  Shut down the display link to conserve power, but request a wakeup call if new images are coming.
			
			CVDisplayLinkStop(displayLink);
			
			[playerItemVideoOutput requestNotificationOfMediaDataChangeWithAdvanceInterval:ADVANCE_INTERVAL_IN_SECONDS];
		}
	}
	
	return kCVReturnSuccess;
}



- (BOOL)update
{
    if (_bLoaded==NO) return NO;
    else{
    
//    // Check our video output for new frames.
//    CMTime outputItemTime = [self.playerItemVideoOutput itemTimeForHostTime:CACurrentMediaTime()];
//    if ([self.playerItemVideoOutput hasNewPixelBufferForItemTime:outputItemTime]) {
//        // Get pixels.
//        if (_latestPixelFrame != NULL) {
//            CVPixelBufferRelease(_latestPixelFrame);
//            _latestPixelFrame = NULL;
//        }
//        _latestPixelFrame = [self.playerItemVideoOutput copyPixelBufferForItemTime:outputItemTime itemTimeForDisplay:NULL];
//        
//        if (self.useTexture) {
//            // Create GL texture.
//            if (_latestTextureFrame != NULL) {
//                CVOpenGLTextureRelease(_latestTextureFrame);
//                _latestTextureFrame = NULL;
//                CVOpenGLTextureCacheFlush(_textureCache, 0);
//            }
//            
//            CVReturn err = CVOpenGLTextureCacheCreateTextureFromImage(NULL, _textureCache, _latestPixelFrame, NULL, &_latestTextureFrame);
//            if (err != noErr) {
//                NSLog(@"Error creating OpenGL texture %d", err);
//            }
//        }
    
        // Update time.
        _currentTime = self.qPlayer.currentItem.currentTime;
        _duration = self.qPlayer.currentItem.duration;
        
        
        return YES;
    }
    
//    return NO;
}

//--------------------------------------------------------------
- (void)playerItemDidReachEnd:(NSNotification *)notification
{
    NSLog(@"ended clip");
    
    _bFinished=YES;

    
//    [self advanceVideo];
//    [self.qPlayer advanceToNextItem];
//    
//    int qSize=[[_queuePlayer items] count];
//    NSLog(@"array size %i",qSize);
//    
//    _currentTime = kCMTimeZero;
//    [self.qPlayer.currentItem addOutput:self.playerItemVideoOutput];
//    
//    [[NSNotificationCenter defaultCenter]
//     addObserver:self
//     selector:@selector(playerItemDidReachEnd:)
//     name:AVPlayerItemDidPlayToEndTimeNotification
//     object:[self.qPlayer currentItem]];
    
//     return _bFinished;

}

-(void)makeActive{

    _currentTime=kCMTimeZero;

}

- (void)pixels:(unsigned char *)outbuf
{
//    @autoreleasepool {
//        if(_newFrame==YES){
//        
//        if (_latestPixelFrame == NULL) return;
//        
//        if ((NSInteger)self.width != CVPixelBufferGetWidth(_latestPixelFrame) || (NSInteger)self.height != CVPixelBufferGetHeight(_latestPixelFrame)) {
//            NSLog(@"CoreVideo pixel buffer is %ld x %ld while self reports size of %ld x %ld. This is most likely caused by a non-square pixel video format such as HDV. Open this video in texture only mode to view it at the appropriate size",
//                  CVPixelBufferGetWidth(_latestPixelFrame), CVPixelBufferGetHeight(_latestPixelFrame), (long)self.width, (long)self.height);
//            return;
//        }
//        
//        if (CVPixelBufferGetPixelFormatType(_latestPixelFrame) != kCVPixelFormatType_32ARGB) {
//            NSLog(@"QTKitMovieRenderer - Frame pixelformat not kCVPixelFormatType_32ARGB: %d, instead %ld", kCVPixelFormatType_32ARGB, (long)CVPixelBufferGetPixelFormatType(_latestPixelFrame));
//            return;
//        }
//        
//        CVPixelBufferLockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
//        //If we are using alpha, the ofxAVFVideoPlayer class will have allocated a buffer of size
//        //video.width * video.height * 4
//        //CoreVideo creates alpha video in the format ARGB, and openFrameworks expects RGBA,
//        //so we need to swap the alpha around using a vImage permutation
//        vImage_Buffer src = {
//            CVPixelBufferGetBaseAddress(_latestPixelFrame),
//            CVPixelBufferGetHeight(_latestPixelFrame),
//            CVPixelBufferGetWidth(_latestPixelFrame),
//            CVPixelBufferGetBytesPerRow(_latestPixelFrame)
//        };
//        vImage_Error err;
//        //If we are are doing RGB then ofxAVFVideoPlayer will have created a buffer of size video.width * video.height * 3
//        //so we use vImage to copy them into the out buffer
//
//            vImage_Buffer dest = { outbuf, self.height, self.width, self.width * 3 };
//            err = vImageConvert_ARGB8888toRGB888(&src, &dest, 0);
//        
//        CVPixelBufferUnlockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
//        
//        NSLog(@"Pixels");
//        
//        if (err != kvImageNoError) {
//            NSLog(@"Error in Pixel Copy vImage_error %ld", err);
//        }
//            _newFrame=NO;
//        }
//    }
}

//--------------------------------------------------------------
- (BOOL)textureAllocated
{
    return self.useTexture && _latestTextureFrame != NULL;
}

//--------------------------------------------------------------
- (GLuint)textureID
{
    return CVOpenGLTextureGetName(_latestTextureFrame);
}

//--------------------------------------------------------------
- (GLenum)textureTarget
{
    return CVOpenGLTextureGetTarget(_latestTextureFrame);
}

//--------------------------------------------------------------
- (void)bindTexture
{
    if (!self.textureAllocated) return;
    
	GLuint texID = [self textureID];
	GLenum target = [self textureTarget];
	
	glEnable(target);
	glBindTexture(target, texID);
}

//--------------------------------------------------------------
- (void)unbindTexture
{
    if (!self.textureAllocated) return;
	
	GLenum target = [self textureTarget];
	glDisable(target);
}

//--------------------------------------------------------------
- (double)duration
{
    return CMTimeGetSeconds(_duration);
}

//--------------------------------------------------------------
- (int)totalFrames
{
    return self.duration * self.frameRate;
}

//--------------------------------------------------------------
- (double)currentTime
{
    return CMTimeGetSeconds(_currentTime);
}

//--------------------------------------------------------------
- (void)setCurrentTime:(double)currentTime
{
    [_queuePlayer seekToTime:CMTimeMakeWithSeconds(currentTime, _duration.timescale)];
}

//--------------------------------------------------------------
- (int)currentFrame
{
    return self.currentTime * self.frameRate;
}

//--------------------------------------------------------------
- (void)setCurrentFrame:(int)currentFrame
{
    float position = currentFrame / (float)self.totalFrames;
    [self setPosition:position];
}

//--------------------------------------------------------------
- (double)position
{
    return self.currentTime / self.duration;
}

//--------------------------------------------------------------
- (void)setPosition:(double)position
{
    double time = self.duration * position;
    //    [self.player seekToTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];
    [_queuePlayer seekToTime:CMTimeMakeWithSeconds(time, _duration.timescale)];
}

//--------------------------------------------------------------
- (void)setPlaybackRate:(double)playbackRate
{
    _playbackRate = playbackRate;
    [_queuePlayer setRate:_playbackRate];
}

//--------------------------------------------------------------
- (float)volume
{
    return self.qPlayer.volume;
}

//--------------------------------------------------------------
- (void)setVolume:(float)volume
{
    self.qPlayer.volume = volume;
}

//-(unsigned char *) pix{
//    return self.pix;
//}

//--------------------------------------------------------------
- (double)width
{
    return _videoSize.width;
}

//--------------------------------------------------------------
- (double)height
{
    return _videoSize.height;
}

//--------------------------------------------------------------
- (BOOL)bFinished
{
    return _bFinished;
}

-(BOOL) bLoaded
{
    return _bLoaded;
}

-(BOOL) bNewFrame
{
    return _newFrame;
}


//--------------------------------------------------------------
- (void)dealloc
{
	self.playerItemVideoOutput = nil;
    
	if (_textureCache != NULL) {
		CVOpenGLTextureCacheRelease(_textureCache);
		_textureCache = NULL;
	}
	if (_latestTextureFrame != NULL) {
		CVOpenGLTextureRelease(_latestTextureFrame);
		_latestTextureFrame = NULL;
	}
	if (_latestPixelFrame != NULL) {
		CVPixelBufferRelease(_latestPixelFrame);
		_latestPixelFrame = NULL;
	}
	
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	
    if (self.playerItem) {
        [self.playerItem removeObserver:self forKeyPath:@"status"];
        self.playerItem = nil;
    }
    
    [self.qPlayer replaceCurrentItemWithPlayerItem:nil];
    self.qPlayer = nil;
    
    [super dealloc];
}
@end


@implementation AVQueueRenderer (AVPlayerItemOutputPullDelegate)

- (void)outputMediaDataWillChange:(AVPlayerItemOutput *)sender
{
	// Start running again.
	_lastHostTime = CVGetCurrentHostTime();
	
	CVDisplayLinkStart(_displayLink);
}

@end
