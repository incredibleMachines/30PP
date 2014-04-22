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


#import "GLView.h"
#import "ofxCocoa.h"

using namespace MSA;
using namespace ofxCocoa;


@implementation GLView

@synthesize player=_player;
@synthesize playerItem=_playerItem;

@synthesize playerItemVideoOutput = _playerItemVideoOutput;

@synthesize bLoaded = _bLoaded;
@synthesize bPaused = _bPaused;
@synthesize bFinished=_bFinished;

@synthesize videoFrameRate = _frameRate;
@synthesize playbackRate = _playbackRate;

@synthesize useDisplayLink;
@synthesize windowMode;
@synthesize openGLContext;
@synthesize pixelFormat;
@synthesize captureExternalMouseEvents;


//------ DISPLAY LINK STUFF ------
-(CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime {
    
    /***** 30PP *****/
    
    CMTime outputItemTime = [self.playerItemVideoOutput itemTimeForCVTimeStamp:*outputTime];
    
    if ([self.playerItemVideoOutput hasNewPixelBufferForItemTime:outputItemTime]){
        
        glEnable(GL_MULTISAMPLE);
        glMatrixMode(GL_PROJECTION);
		
        CVPixelBufferRef pixBuff = [self.playerItemVideoOutput copyPixelBufferForItemTime:outputItemTime itemTimeForDisplay:NULL];
        
        if (_latestTextureFrame != NULL) {
            CVOpenGLTextureRelease(_latestTextureFrame);
            _latestTextureFrame = NULL;
            CVOpenGLTextureCacheFlush(_textureCache, 0);
        }
        
        CVReturn err = CVOpenGLTextureCacheCreateTextureFromImage(NULL, _textureCache, pixBuff, NULL, &_latestTextureFrame);
        
        if (err != noErr) {
            NSLog(@"Error creating OpenGL texture %d", err);
        }
        
        CVPixelBufferRelease(pixBuff);
	}
    
    /***** 30PP *****/
    
    [self updateAndDraw];
    return kCVReturnSuccess;
}


// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext){
    
    CVReturn result = [(GLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
    
}


-(void)setupDisplayLink {
    
	NSLog(@"glView::setupDisplayLink");
	// Create a display link capable of being used with all active displays
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	
	// Set the renderer output callback function
	CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
	
	// Set the display link for the current renderer
	CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
	// start it
	CVDisplayLinkStart(displayLink);
}

-(void)releaseDisplayLink {
	NSLog(@"glView::releaseDisplayLink");
    
	CVDisplayLinkStop(displayLink);
	CVDisplayLinkRelease(displayLink);
	displayLink = 0;
}


// --------------------------------

-(void)setupTimer {
	NSLog(@"glView::setupTimer");
	
	float dur = targetFrameRate > 0 ? 1.0f /targetFrameRate : 0.001f;
	
	timer = [[NSTimer timerWithTimeInterval:dur target:self selector:@selector(updateAndDraw) userInfo:nil repeats:YES] retain];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSModalPanelRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
}

-(void)releaseTimer {
	NSLog(@"glView::releaseTimer");
	
	[timer invalidate];
	timer = 0;
}

// --------------------------------


-(void) setSyncToDisplayLink:(BOOL)b {
	[self stopAnimation];
	useDisplayLink = b;
	[self startAnimation];
}


-(void)startAnimation {
	NSLog(@"glView::startAnimation using displayLink %@", useDisplayLink ? @"YES" : @"NO");
	
	if(!isAnimating /*&& displayLink && !CVDisplayLinkIsRunning(displayLink)*/){
		isAnimating = true;
		
		if(useDisplayLink){
			[self setupDisplayLink];
		} else {
			[self setupTimer];
		}
	}
}

-(void)stopAnimation {
	NSLog(@"glView::stopAnimation using displayLink %@", useDisplayLink ? @"YES" : @"NO");
	if(isAnimating /*&& displayLink && CVDisplayLinkIsRunning(displayLink)*/) {
		isAnimating = false;
		
		if(useDisplayLink) {
			[self releaseDisplayLink];
		} else {
			[self releaseTimer];
		}
	}
}

-(void)toggleAnimation {
	if (isAnimating) [self stopAnimation];
	else [self startAnimation];
}

-(void)setFrameRate:(float)rate {
	NSLog(@"glView::setFrameRate %f", rate);
	[self stopAnimation];
	targetFrameRate = rate;
	[self startAnimation];
}


-(void)reshape {
	// This method will be called on the main thread when resizing, but we may be drawing on a secondary thread through the display link
	// Add a mutex around to avoid the threads accessing the context simultaneously
	if(useDisplayLink) CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
	
	// Delegate to the scene object to update for a change in the view size
	//	[[controller scene] setViewportRect:[self bounds]];// TODO
	[[self openGLContext] update];
	
	if(useDisplayLink) CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}


-(void)drawRect:(NSRect)dirtyRect {
}


-(void)updateAndDraw {
    [[NSCursor crosshairCursor] set];
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	// This method will be called on both the main thread (through -drawRect:) and a secondary thread (through the display link rendering loop)
	// Also, when resizing the view, -reshape is called on the main thread, but we may be drawing on a secondary thread
	// Add a mutex around to avoid the threads accessing the context simultaneously
    
	if(useDisplayLink) CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
	
	// Make sure we draw to the right context
	[[self openGLContext] makeCurrentContext];
	
	appWindow()->updateAndDraw();
	
	[[self openGLContext] flushBuffer];
	
	if(useDisplayLink) CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
	
	[pool release];
}


-(id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context {
    
	
    NSLog(@"GLView::initWithFrame %@", NSStringFromRect(frameRect));
	
	isAnimating		= false;
	useDisplayLink	= true;
	
	pixelFormat = nil;
    
    NSLog(@"%@", appWindow()->context);
	
	// Initialized at AppWindow::setupOpenGL
	if(appWindow()->initSettings().numFSAASamples) {
		NSOpenGLPixelFormatAttribute attribs[] = {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)24,
            NSOpenGLPFAMultisample,
            NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
            NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)8,
            NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)8,
//         NSOpenGLPFAOpenGLProfile,
//         NSOpenGLProfileVersion3_2Core,
//        (NSOpenGLPixelFormatAttribute)0
        };
		
		pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
        NSLog(@"      Multisampling supported");
        glEnable(GL_MULTISAMPLE);
	}
    
	openGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:context];
	
	if (self = [super initWithFrame:frameRect]) {
		[[self openGLContext] makeCurrentContext];
//        [self setOpenGLContext:(NSOpenGLContext) ]
        
		// set surface opacity
		GLint i = appWindow()->initSettings().isOpaque;
		[[self openGLContext] setValues:&i forParameter:NSOpenGLCPSurfaceOpacity];
		
		// enable vertical sync
		i = 1;
		[[self openGLContext] setValues:&i forParameter:NSOpenGLCPSwapInterval];
        
		
		// Look for changes in view size
		// Note, -reshape will not be called automatically on size changes because NSView does not export it to override
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(reshape)
													 name:NSViewGlobalFrameDidChangeNotification
												   object:self];
        
        [[NSNotificationCenter defaultCenter] addObserver: self
                                                 selector: @selector (didChangeScreenParameters:)
                                                     name: NSApplicationDidChangeScreenParametersNotification
                                                   object: nil];
        
        
        
	} else {
        cout << "ERROR SETTING UP WINDOW" << endl;
    }
    
    NSLog(@"%s %s", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    NSArray *screens = [NSScreen screens];
    NSLog(@"Screen Count: %i",[screens count]);
    _numScreens=[screens count];
    
    ofGLReadyCallback();
	
	return self;
}

-(id) initWithFrame:(NSRect)frameRect {
	self = [self initWithFrame:frameRect shareContext:nil];
	return self;
}


- (void) didChangeScreenParameters: (NSNotification *) notification
{
    NSArray *screens = [NSScreen screens];
    
    
    NSLog(@"New Screen Count: %i",[screens count]);
    
    for(int i=0; i<[screens count];i++){
        NSScreen *screen = [screens objectAtIndex:i];
        NSRect screenFrame = [screen frame];
        NSLog(@"Width: %f", NSWidth(screenFrame));
    }
    
    NSLog(@"Total Width: %f",NSWidth(rectForAllScreens()));
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    
    ofSetWindowPosition(0,0);
    ofSetWindowShape(NSWidth(rectForAllScreens()),NSHeight(rectForAllScreens()));
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    
    _numScreens=[screens count];
}


-(void)lockFocus {
	[super lockFocus];
	if ([[self openGLContext] view] != self)
		[[self openGLContext] setView:self];
}

-(void)dealloc {
	[self stopAnimation];
	
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:[self.player currentItem]];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSViewGlobalFrameDidChangeNotification object:self];
    
    [[NSNotificationCenter defaultCenter] removeObserver: self
                                                    name: NSApplicationDidChangeScreenParametersNotification
                                                  object: self];

    
    CVOpenGLTextureRelease(_latestTextureFrame);
    CVOpenGLTextureCacheFlush(_textureCache, 0);
	[openGLContext release];
	[pixelFormat release];
    [_player pause];
    [_player release];
    
    [super dealloc];
}



-(void)awakeFromNib {
	NSLog(@"GLView::awakeFromNib, window:%@",[self window]);
	[[self window] setAcceptsMouseMovedEvents:YES];
    
}

-(void)goFullscreen:(NSScreen*)screen {
	NSLog(@"GLView::goFullscreen: %@", screen);
	windowMode = OF_FULLSCREEN;
	[self stopAnimation];
	
	if([self respondsToSelector:@selector(isInFullScreenMode)]) {
		[self enterFullScreenMode:screen
					  withOptions:[NSDictionary dictionaryWithObjectsAndKeys:
								   [NSNumber numberWithBool: NO], NSFullScreenModeAllScreens,
								   nil]
		 ];
		
	}
	
	[self startAnimation];
}


// ---------------------------------
-(void)goWindow{
	NSLog(@"GLView::goWindow");
	
	windowMode = OF_WINDOW;
	[self stopAnimation];
	
	if([self respondsToSelector:@selector(isInFullScreenMode)] && [self isInFullScreenMode]){
		[self exitFullScreenModeWithOptions:nil];
		
	} else {
		SetSystemUIMode(kUIModeNormal, NULL);
		if(savedWindowFrame.size.width == 0 || savedWindowFrame.size.height == 0) {
			savedWindowFrame.size = NSMakeSize(1024, 768);
		}
		
		[[self window] setFrame:savedWindowFrame display:YES animate:NO];
		[[self window] setLevel:NSNormalWindowLevel];
		
	}
	
	[self startAnimation];
}

-(void)toggleFullscreen {
	if(windowMode == OF_WINDOW) [self goFullscreen:currentScreen()];
	else [self goWindow];
}




-(BOOL)acceptsFirstResponder {
	return YES;
}

-(BOOL)becomeFirstResponder {
	return  YES;
}

-(BOOL)resignFirstResponder {
	return YES;
}


#pragma mark Events

-(void)keyDown:(NSEvent *)theEvent {
    //	NSLog(@"%@", theEvent);
    
    if ([theEvent modifierFlags] & NSShiftKeyMask) {
        //        NSLog(@"Shift key pressed");
        ofNotifyKeyPressed(OF_KEY_SHIFT);
    }
    else{
        //        NSLog(@"Shift key not pressed");
        ofNotifyKeyReleased(OF_KEY_SHIFT);
    }
    
    if ([theEvent modifierFlags] & NSCommandKeyMask) {
        //        NSLog(@"CMD key pressed");
        ofNotifyKeyPressed(OF_KEY_COMMAND);
    }
    else{
        //        NSLog(@"CMD key not pressed");
        ofNotifyKeyReleased(OF_KEY_COMMAND);
    }
    
    NSString *characters = [theEvent characters];
    if ([characters length]) {
        unichar key = [characters characterAtIndex:0];
        //            NSLog(@"key code pressed %hu", key);
        switch(key) {
            case OF_KEY_ESC:
                OF_EXIT_APP(0);
                break;
                
            case 63232:
                key = OF_KEY_UP;
                break;
                
            case 63235:
                key = OF_KEY_RIGHT;
                break;
                
            case 63233:
                key = OF_KEY_DOWN;
                break;
                
            case 63234:
                key = OF_KEY_LEFT;
                break;
                
        }
        ofNotifyKeyPressed(key);
    }
}

-(void)keyUp:(NSEvent *)theEvent {
	NSString *characters = [theEvent characters];
	if ([characters length]) {
		unichar key = [characters characterAtIndex:0];
		ofNotifyKeyReleased(key);
	}
}

-(ofPoint) ofPointFromEvent:(NSEvent*)theEvent {
	NSPoint p = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	return ofPoint(p.x, self.frame.size.height - p.y, 0);
}

/***** 30PP *****/
- (NSDictionary *)pixelBufferAttributes
{
    return @{
             (NSString *)kCVPixelBufferOpenGLCompatibilityKey : [NSNumber numberWithBool:YES],
             (NSString *)kCVPixelBufferPixelFormatTypeKey     : [NSNumber numberWithInt:kCVPixelFormatType_32ARGB]
             };
}

-(void)initPlayer:(NSString *)path ID:(int)ID{
    _texID=ID;
    NSLog(@"VideoPath:%@",path);
    NSLog(@"init");
    _bLoaded = NO;
    _bPaused = NO;
    _bFinished = NO;
    _bPixelsAllocated=NO;
    
    _frameRate = 0.0;
    _playbackRate = 1.0;
    
    // Create and attach video output. 10.8 Only!!!
    self.playerItemVideoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:[self pixelBufferAttributes]];
    [self.playerItemVideoOutput autorelease];
    if (self.playerItemVideoOutput) {
        NSLog(@"generated video output");
        [(AVPlayerItemVideoOutput *)self.playerItemVideoOutput setSuppressesPlayerRendering:YES];
    }
    
    NSURL *url=[NSURL fileURLWithPath:[path stringByStandardizingPath]];
    
    
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
                
                self.playerItem = [AVPlayerItem playerItemWithAsset:asset];
                
                NSLog(@"create player");
                
                self.player=[[AVPlayer alloc] initWithPlayerItem:self.playerItem];
                
                NSLog(@"PLAYER CREATED");;
                
                // Notify this object when the player reaches the end
                // This allows us to loop the video
                [[NSNotificationCenter defaultCenter]
                 addObserver:self
                 selector:@selector(playerItemDidReachEnd:)
                 name:AVPlayerItemDidPlayToEndTimeNotification
                 object:[self.player currentItem]];
                
                [[self.player currentItem] addOutput:self.playerItemVideoOutput];
                
                
                // Create CVOpenGLTextureCacheRef for optimal CVPixelBufferRef to GL texture conversion.
                if (!_textureCache) {
                    
                    
                    CVReturn err = CVOpenGLTextureCacheCreate(kCFAllocatorDefault, NULL,
                                                              CGLGetCurrentContext(), CGLGetPixelFormat(CGLGetCurrentContext()),
                                                              NULL, &_textureCache);
                    
                    if (err != noErr) {
                        NSLog(@"Error at CVOpenGLTextureCacheCreate %d", err);
                    }
                }
                
                [self.player play];
                
            }
            _bLoaded=YES;
        });
    }];
}

- (void)playerItemDidReachEnd:(NSNotification *)notification
{
    [self.player seekToTime:kCMTimeZero];
    [self.player play];
}

-(void)startPlayer
{
    [self.player setRate:1.0];
}

-(void)pausePlayer
{
    [self.player setRate:0.0];
}

-(void)setTime:(float)millis
{
    [self.player seekToTime:CMTimeMakeWithSeconds(millis, _duration.timescale)];
}

/***** 30PP *****/



//------------------------------------------------------------
// ADDED BR 7/31/11

-(void) setCaptureExternalMouseEvents:(BOOL)b {
	captureExternalMouseEvents = b;
	
	if (captureExternalMouseEvents){
		mouseMoveHandler	= [NSEvent addGlobalMonitorForEventsMatchingMask:NSMouseMovedMask handler:^(NSEvent * mouseEvent) {
			//NSLog(@"Mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
			[self mouseMoveOutside:mouseEvent];
		}];
		
		// down
		
		leftMouseDownHandler	= [NSEvent addGlobalMonitorForEventsMatchingMask:NSLeftMouseDownMask handler:^(NSEvent * mouseEvent) {
            //			NSLog(@"Mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
			[self mouseDownOutside:mouseEvent];
		}];
		
		rightMouseDownHandler	= [NSEvent addGlobalMonitorForEventsMatchingMask:NSRightMouseDownMask handler:^(NSEvent * mouseEvent) {
			//NSLog(@"Right mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
			[self rightMouseDownOutside:mouseEvent];
		}];
		
		// up
		
		leftMouseUpHandler		= [NSEvent addGlobalMonitorForEventsMatchingMask:NSLeftMouseUpMask handler:^(NSEvent * mouseEvent) {
			//NSLog(@"Mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
			[self mouseUpOutside:mouseEvent];
		}];
		
		rightMouseUpHandler		= [NSEvent addGlobalMonitorForEventsMatchingMask:NSRightMouseUpMask  handler:^(NSEvent * mouseEvent) {
			//NSLog(@"Right mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
			[self rightMouseUpOutside:mouseEvent];
		}];
		
		// drag
		
		leftMouseDraggedHandler	= [NSEvent addGlobalMonitorForEventsMatchingMask:NSLeftMouseDraggedMask handler:^(NSEvent * mouseEvent) {
			//NSLog(@"Mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
			[self mouseDraggedOutside:mouseEvent];
		}];
		
		rightMouseDraggedHandler = [NSEvent addGlobalMonitorForEventsMatchingMask:NSRightMouseDraggedMask handler:^(NSEvent * mouseEvent) {
			//NSLog(@"Mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
			[self rightMouseDraggedOutside:mouseEvent];
		}];
		
		externalMouseEventsActive = true;
	} else if (externalMouseEventsActive) {
		
		
		[NSEvent removeMonitor:mouseMoveHandler];
		mouseMoveHandler = nil;
		
		// down
		
		[NSEvent removeMonitor:leftMouseDownHandler];
		leftMouseDownHandler = nil;
		[NSEvent removeMonitor:rightMouseDownHandler];
		rightMouseDownHandler = nil;
		
		// up
		
		[NSEvent removeMonitor:leftMouseUpHandler];
		leftMouseUpHandler = nil;
		[NSEvent removeMonitor:rightMouseUpHandler];
		rightMouseUpHandler = nil;
		
		// drag
		
		[NSEvent removeMonitor:leftMouseDraggedHandler];
		leftMouseDraggedHandler = nil;
		[NSEvent removeMonitor:rightMouseDraggedHandler];
		rightMouseDraggedHandler = nil;
		
		externalMouseEventsActive = false;
	}
}


-(void)mouseDown:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMousePressed(p.x, p.y, 0);
    
    if ([theEvent modifierFlags] & NSShiftKeyMask) {
        ofNotifyKeyPressed(OF_KEY_SHIFT);
    }
    else{
        ofNotifyKeyReleased(OF_KEY_SHIFT);
    }
    
    if ([theEvent modifierFlags] & NSCommandKeyMask) {
        ofNotifyKeyPressed(OF_KEY_COMMAND);
    }
    else{
        ofNotifyKeyReleased(OF_KEY_COMMAND);
    }
}

-(void)rightMouseDown:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMousePressed(p.x, p.y, 1);
}

-(void)otherMouseDown:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMousePressed(p.x, p.y, 2);
}

-(void)mouseMoved:(NSEvent *)theEvent{
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMouseMoved(p.x, p.y);
}

-(void)mouseUp:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMouseReleased(p.x, p.y, 0);
    
    if ([theEvent modifierFlags] & NSShiftKeyMask) {
        ofNotifyKeyPressed(OF_KEY_SHIFT);
    }
    else{
        ofNotifyKeyReleased(OF_KEY_SHIFT);
    }
    
    if ([theEvent modifierFlags] & NSCommandKeyMask) {
        ofNotifyKeyPressed(OF_KEY_COMMAND);
    }
    else{
        ofNotifyKeyReleased(OF_KEY_COMMAND);
    }
    
}

-(void)rightMouseUp:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMouseReleased(p.x, p.y, 1);
}

-(void)otherMouseUp:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMouseReleased(p.x, p.y, 2);
}

-(void)mouseDragged:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMouseDragged(p.x, p.y, 0);
}

-(void)rightMouseDragged:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMouseDragged(p.x, p.y, 1);
}

-(void)otherMouseDragged:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromEvent:theEvent];
	ofNotifyMouseDragged(p.x, p.y, 2);
}

-(void)scrollWheel:(NSEvent *)theEvent {
	//	NSLog(@"scrollWheel");
	
	//	float wheelDelta = [theEvent deltaX] +[theEvent deltaY] + [theEvent deltaZ];
	//	if (wheelDelta)
	//	{
	//		GLfloat deltaAperture = wheelDelta * -camera.aperture / 200.0f;
	//
	//	}
}

//------------------------------------------------------------
// BR: external mouse events

//------------------------------------------------------------
-(ofPoint) ofPointFromOutsideEvent:(NSEvent*)theEvent {
	NSPoint p = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	return ofPoint(p.x, ofGetScreenHeight() - p.y, 0);
}

//------------------------------------------------------------
-(void)mouseMoveOutside:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromOutsideEvent:theEvent];
	notifyMouseMovedOutside(p.x, p.y);
}

//------------------------------------------------------------
-(void)mouseDownOutside:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromOutsideEvent:theEvent];
	notifyMousePressedOutside(p.x, p.y, 0);
}

//------------------------------------------------------------
-(void)rightMouseDownOutside:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromOutsideEvent:theEvent];
	notifyMousePressedOutside(p.x, p.y, 1);
}

//------------------------------------------------------------
-(void)mouseDraggedOutside:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromOutsideEvent:theEvent];
	notifyMouseDraggedOutside(p.x, p.y, 0);
}

//------------------------------------------------------------
-(void)rightMouseDraggedOutside:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromOutsideEvent:theEvent];
	notifyMouseDraggedOutside(p.x, p.y, 1);
}

//------------------------------------------------------------
-(void)mouseUpOutside:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromOutsideEvent:theEvent];
	notifyMouseReleasedOutside(p.x, p.y, 0);
}

//------------------------------------------------------------
-(void)rightMouseUpOutside:(NSEvent *)theEvent {
	ofPoint p = [self ofPointFromOutsideEvent:theEvent];
	notifyMouseReleasedOutside(p.x, p.y, 1);
}


/***** 30PP *****/

//--------------------------------------------------------------
- (double)duration
{
    return CMTimeGetSeconds(_duration);
}

//--------------------------------------------------------------
- (int)totalFrames
{
    return self.duration * self.videoFrameRate;
}

//--------------------------------------------------------------
- (double)currentTime
{
    return CMTimeGetSeconds(_currentTime);
}

//--------------------------------------------------------------
- (void)setCurrentTime:(double)currentTime
{
    [_player seekToTime:CMTimeMakeWithSeconds(currentTime, _duration.timescale)];
}

//--------------------------------------------------------------
- (int)currentFrame
{
    return self.currentTime * self.videoFrameRate;
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
    [_player seekToTime:CMTimeMakeWithSeconds(time, _duration.timescale)];
}

//--------------------------------------------------------------
- (void)setPlaybackRate:(double)playbackRate
{
    _playbackRate = playbackRate;
    [_player setRate:_playbackRate];
}

//--------------------------------------------------------------
- (float)volume
{
    return self.player.volume;
}

//--------------------------------------------------------------
- (void)setVolume:(float)volume
{
    self.player.volume = volume;
}

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

-(int) numScreens
{
    return _numScreens;
}

//--------------------------------------------------------------
- (GLuint)textureID
{
    if(_latestTextureFrame){
        return CVOpenGLTextureGetName(_latestTextureFrame);
    }
    else{
        return 1;
    }
}

/***** 30PP *****/

@end
