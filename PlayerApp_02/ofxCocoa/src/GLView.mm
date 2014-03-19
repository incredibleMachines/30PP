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
    
        CMTime outputItemTime = [self.playerItemVideoOutput itemTimeForCVTimeStamp:*outputTime];
    
    if ([self.playerItemVideoOutput hasNewPixelBufferForItemTime:outputItemTime]){
        
//		self->_lastHostTime = outputTime->hostTime;
//      Copy the pixel buffer to be displayed next and add it to AVSampleBufferDisplayLayer for display
//        CGLLockContext(CGLGetCurrentContext());
        glEnable(GL_MULTISAMPLE);
		
        CVPixelBufferRef pixBuff = [self.playerItemVideoOutput copyPixelBufferForItemTime:outputItemTime itemTimeForDisplay:NULL];
        
//        _latestBuffer=pixBuff;
//        CFDictionarySetValue(dict, kCVOpenGLBufferTarget, [NSNumber numberWithInt:GL_TEXTURE_2D]);
//        CFDictionarySetValue(dict, kCVOpenGLBufferInternalFormat, [NSNumber numberWithInt:GL_RGB]);
//        CFDictionarySetValue(dict, kCVOpenGLBufferMaximumMipmapLevel, [NSNumber numberWithInt:6]);
//        CFDictionarySetValue(dict, kCVOpenGLBufferWidth, [NSNumber numberWithInt:[self width]]);
//        CFDictionarySetValue(dict, kCVOpenGLBufferHeight, [NSNumber numberWithInt:[self height]]);
        
        CVBufferSetAttachment(pixBuff, kCVOpenGLBufferTarget, [NSNumber numberWithInt:GL_TEXTURE_2D],kCVAttachmentMode_ShouldPropagate);
        
        CVOpenGLBufferRef _latestBuffer;
        
        CVOpenGLBufferPoolCreateOpenGLBuffer(NULL, _buffer, &_latestBuffer);

        CVOpenGLBufferAttach(pixBuff, CGLGetCurrentContext(), 0, 0, 0);
        
//        NSLog(@"Mipmap Level is %@",  (CFNumberRef) CFDictionaryGetValue(CVOpenGLBufferGetAttributes(_latestBuffer), kCVOpenGLBufferMaximumMipmapLevel));
        
        /***** 30PP *****/
//        NSLog(@"Pix Buff width: %lu",CVPixelBufferGetWidth(pixBuff));
        
        if (_latestTextureFrame != NULL) {
            CVOpenGLTextureRelease(_latestTextureFrame);
            _latestTextureFrame = NULL;
            CVOpenGLTextureCacheFlush(_textureCache, 0);
        }
        
//        if (_latestBuffer != NULL) {
//            CVOpenGLBufferRelease(_latestBuffer);
//            _latestBuffer = NULL;
//        }
        
        CVReturn err = CVOpenGLTextureCacheCreateTextureFromImage(NULL, _textureCache, pixBuff, NULL, &_latestTextureFrame);
        
        if (err != noErr) {
            NSLog(@"Error creating OpenGL texture %d", err);
        }
        
        NSLog(@"Tex ID %i", CVOpenGLTextureGetName(_latestTextureFrame));
        NSLog(@"Tex Target %i", CVOpenGLTextureGetTarget(_latestTextureFrame));
        NSLog(@"GLBufferTarget %@", (NSNumber *)CVBufferGetAttachment(pixBuff, kCVOpenGLBufferTarget, NULL));
        
        CVPixelBufferRelease(pixBuff);
        CVOpenGLBufferRelease(_latestBuffer);
        
//        CGLUnlockContext(CGLGetCurrentContext());
        
        
//        
//        glBindTexture(GL_TEXTURE_2D,  CVOpenGLTextureGetName(_latestTextureFrame));
//        
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, self.width, self.height, 0, GL_RGB, GL_UNSIGNED_BYTE, _latestBuffer);
//        
//        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
//        
//        
//        //set environment, not using currenty but just incase we change our env elsewhere
//        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//        
//        glEnable(GL_TEXTURE_2D);
//        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
//
//        //set params, GL_LINEAR_MIPMAP_LINER is trilinear which means greatest quality
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//        //anisotropy unblurs our mip maps, basically sets "how much" we want antialias with higher param being less blur
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
//        
//        glDisable(GL_TEXTURE_2D);
    
        
//        GLuint glID=self->_texID;
//        
//        if(self->_bPixelsAllocated==NO){
//            self->_bPixelsAllocated=YES;
//            glGenTextures(1, &glID);
//        }
        
//            bind texture
        

        
        

        
//        if(CVPixelBufferGetWidth(pixBuff)!=0&&self->_bPixelsAllocated==NO){
//            self->_bPixelsAllocated=YES;
//            
//            if ((NSInteger)self.width != CVPixelBufferGetWidth(pixBuff) || (NSInteger)self.height != CVPixelBufferGetHeight(pixBuff)) {
//                NSLog(@"LAUNCH CoreVideo pixel buffer is %ld x %ld while self reports size of %ld x %ld.",
//                      CVPixelBufferGetWidth(pixBuff), CVPixelBufferGetHeight(pixBuff), (long)self.width, (long)self.height);
//                return;
//            }
//            
//            if (CVPixelBufferGetPixelFormatType(pixBuff) != kCVPixelFormatType_32ARGB) {
//                NSLog(@"QTKitMovieRenderer - Frame pixelformat not kCVPixelFormatType_32ARGB: %d, instead %ld", kCVPixelFormatType_32ARGB, (long)CVPixelBufferGetPixelFormatType(pixBuff));
//                return;
//            }
//            
//            CVPixelBufferLockBaseAddress(pixBuff, kCVPixelBufferLock_ReadOnly);
//            //If we are using alpha, the ofxAVFVideoPlayer class will have allocated a buffer of size
//            //video.width * video.height * 4
//            //CoreVideo creates alpha video in the format ARGB, and openFrameworks expects RGBA,
//            //so we need to swap the alpha around using a vImage permutation
//            vImage_Buffer src = {
//                CVPixelBufferGetBaseAddress(pixBuff),
//                CVPixelBufferGetHeight(pixBuff),
//                CVPixelBufferGetWidth(pixBuff),
//                CVPixelBufferGetBytesPerRow(pixBuff)
//            };
//            vImage_Error err;
//            //If we are are doing RGB then ofxAVFVideoPlayer will have created a buffer of size video.width * video.height * 3
//            //so we use vImage to copy them into the out buffer
//            
//            int size=[self width] * [self height] *3;
//            NSLog(@"char array size: %i", size);
//            
//            _pix = (unsigned char*) calloc(size, sizeof(unsigned char));
//            
//            vImage_Buffer dest = { _pix, self.height, self.width, self.width * 3 };
//            err = vImageConvert_ARGB8888toRGB888(&src, &dest, 0);
//            
//            CVPixelBufferUnlockBaseAddress(pixBuff, kCVPixelBufferLock_ReadOnly);
//            
//            //            mesh[_i].glFormat=ofGetGlInternalFormat(mesh[_i].video.getPixelsRef());
//            //            mesh[_i].glType=ofGetGlType(mesh[_i].video.getPixelsRef());
//            //
//            self->_glFormat=GL_RGB;
//            self->_glType=GL_UNSIGNED_BYTE;
//            
//            
//            //allocate texture
//            
//
//            
//            //set our pixel source to determine mip map texel size
//            
//            //create texture
//            GLuint glID=self->_texID;
//            glGenTextures(1, &glID);
//            
//            //bind texture
//            glBindTexture(GL_TEXTURE_2D, self->_texID);
//            
//            //setup mipmaps
//            glTexImage2D(GL_TEXTURE_2D, 0, self->_glType, self.width, self.height, 0, self->_glFormat, self->_glType, _pix);
////            glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
////            
////            //set environment, not using currenty but just incase we change our env elsewhere
////            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
////            
////            //enable GL_TEXTURE_2D
////            glEnable(GL_TEXTURE_2D);
////            
////            //create mipmaps
////            glGenerateMipmap(GL_TEXTURE_2D);
////            
////            //set params, GL_LINEAR_MIPMAP_LINER is trilinear which means greatest quality
////            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////            
////            //anisotropy unblurs our mip maps, basically sets "how much" we want antialias with higher param being less blur
////            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
////            glDisable(GL_TEXTURE_2D);
//            
//            /***** 30PP *****/
//            
//            
//            
//            
//        }
//        
//        else if(CVPixelBufferGetWidth(pixBuff)!=0){
//            
//            
//            if ((NSInteger)self.width != CVPixelBufferGetWidth(pixBuff) || (NSInteger)self.height != CVPixelBufferGetHeight(pixBuff)) {
//                NSLog(@"CoreVideo pixel buffer is %ld x %ld while self reports size of %ld x %ld. This is most likely caused by a non-square pixel video format such as HDV. Open this video in texture only mode to view it at the appropriate size",
//                      CVPixelBufferGetWidth(pixBuff), CVPixelBufferGetHeight(pixBuff), (long)self.width, (long)self.height);
//                return;
//            }
//            
//            if (CVPixelBufferGetPixelFormatType(pixBuff) != kCVPixelFormatType_32ARGB) {
//                NSLog(@"QTKitMovieRenderer - Frame pixelformat not kCVPixelFormatType_32ARGB: %d, instead %ld", kCVPixelFormatType_32ARGB, (long)CVPixelBufferGetPixelFormatType(pixBuff));
//                return;
//            }
//            
//            CVPixelBufferLockBaseAddress(pixBuff, kCVPixelBufferLock_ReadOnly);
//            //If we are using alpha, the ofxAVFVideoPlayer class will have allocated a buffer of size
//            //video.width * video.height * 4
//            //CoreVideo creates alpha video in the format ARGB, and openFrameworks expects RGBA,
//            //so we need to swap the alpha around using a vImage permutation
//            vImage_Buffer src = {
//                CVPixelBufferGetBaseAddress(pixBuff),
//                CVPixelBufferGetHeight(pixBuff),
//                CVPixelBufferGetWidth(pixBuff),
//                CVPixelBufferGetBytesPerRow(pixBuff)
//            };
//            vImage_Error err;
//            //If we are are doing RGB then ofxAVFVideoPlayer will have created a buffer of size video.width * video.height * 3
//            //so we use vImage to copy them into the out buffer
//            
//            vImage_Buffer dest = { _pix, self.height, self.width, self.width * 3 };
//            err = vImageConvert_ARGB8888toRGB888(&src, &dest, 0);
//            
//            CVPixelBufferUnlockBaseAddress(pixBuff, kCVPixelBufferLock_ReadOnly);
//            
//            //bind texture
//            glBindTexture(GL_TEXTURE_2D, self->_texID);
//            
//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, self.width, self.height, self->_glFormat, self->_glType, _pix);
//            
////            glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
////            
////            //environment, in case we change elsewhere
////            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
////            
////            //enable GL_TEXTURE_2D
////            glEnable(GL_TEXTURE_2D);
////            
////            //make mipmaps - REQUIRES GL to be post-3.0
////            glGenerateMipmap(GL_TEXTURE_2D);
////            
////            //set quality - GL_LINEAR_MIPMAP_LINEAR is highest, trilinear
////            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////            
////            //set amount of anisotropic filtering - this undoes the blur of the mipmapping relative to camera
////            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
////            glDisable(GL_TEXTURE_2D);
//            
//            
//            if (err != kvImageNoError) {
//                NSLog(@"Error in Pixel Copy vImage_error %ld", err);
//            }
//            
//            /***** 30PP *****/
//        }
		

	}
    
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
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	// Update the animation
	//	CFAbsoluteTime currentTime = CFAbsoluteTimeGetCurrent();
	//	[[controller scene] advanceTimeBy:(currentTime - [controller renderTime])];
	//	[controller setRenderTime:currentTime];
	
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
    
	

	// Fixing the addon to compile against OF_007
//	cout<<"here"<<endl;
    NSLog(@"%@", appWindow()->context);
//	openGLContext = appWindow()->context;
	
	// Initialized at AppWindow::setupOpenGL
	if(appWindow()->initSettings().numFSAASamples) {
		NSOpenGLPixelFormatAttribute attribs[] = {
            NSOpenGLPFADoubleBuffer,

            NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)24,

            NSOpenGLPFAMultisample,
            NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
            NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)8,

//            NSOpenGLPFAColorSize, 32,
//            NSOpenGLPFAAlphaSize, 8,
//            NSOpenGLPFAAccelerated,
//            NSOpenGLPFAMultiScreen,

////            NSOpenGLPFASampleBuffers, 8,
////            NSOpenGLPFASamples, appWindow()->initSettings().numFSAASamples,

//            NSOpenGLPFANoRecovery, 0,
//            NSOpenGLPFAFullScreen, 1,
//            NSOpenGLPFAOpenGLProfile,
//            NSOpenGLProfileVersion3_2Core,
            (NSOpenGLPixelFormatAttribute)0
};
		
		NSLog(@"   trying Multisampling");
		pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
//		if(pixelFormat) {
			NSLog(@"      Multisampling supported");
			glEnable(GL_MULTISAMPLE);
//		} else {
//			NSLog(@"      Multisampling not supported");
//		}
	}

	
	
//	if(pixelFormat == nil) {
//		NSLog(@"   trying non multisampling");
//		NSOpenGLPixelFormatAttribute attribs[] = {
//			NSOpenGLPFAAccelerated,
//			NSOpenGLPFADoubleBuffer,
//			NSOpenGLPFAMultiScreen,
//			NSOpenGLPFADepthSize, 24,
//			NSOpenGLPFAAlphaSize, 8,
//			NSOpenGLPFAColorSize, 32,
//			NSOpenGLPFANoRecovery,
//			0};		
//		
//		pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
//		glDisable(GL_MULTISAMPLE);
//		if(pixelFormat == nil) {
//			NSLog(@"      not even that. fail");
//		}
//	} 
	
	
	openGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:context];

    
	// End fix 
	
	if (self = [super initWithFrame:frameRect]) {
		[[self openGLContext] makeCurrentContext];
        
		
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
        
        
	} else {
        cout << "ERROR SETTING UP WINDOW" << endl;
    }
    
        NSLog(@"%s %s", glGetString(GL_RENDERER), glGetString(GL_VERSION));
	
//    CGLPixelFormatObj pixelFormatObj;
//    CGLContextObj contextObj;
//	GLint numPixelFormats;
//	
//	// Set up an array of attributes
//	CGLPixelFormatAttribute attribs[] = {
//        kCGLPFADoubleBuffer,
//		kCGLPFAColorSize, (CGLPixelFormatAttribute)24,
//		kCGLPFADepthSize, (CGLPixelFormatAttribute)24,
//        kCGLPFAMultisample,
//        kCGLPFASampleBuffers, (CGLPixelFormatAttribute)1,
//        kCGLPFASamples, (CGLPixelFormatAttribute)16,
//        (CGLPixelFormatAttribute)0
//    };
//	
//	// Create the full-screen context with the attributes listed above
//	// By specifying the non-fullscreen context as the shareContext, we automatically inherit the OpenGL objects (textures, etc) it has defined
//	CGLChoosePixelFormat(attribs, &pixelFormatObj, &numPixelFormats);
//	CGLCreateContext(pixelFormatObj, CGLGetCurrentContext(), &contextObj);
////    CGLSetCurrentContext(contextObj);
//	CGLDestroyPixelFormat(pixelFormatObj);
////    CGLDestroyContext(contextObj);
//    
    ofGLReadyCallback();
	
	return self;
}

-(id) initWithFrame:(NSRect)frameRect {
	self = [self initWithFrame:frameRect shareContext:nil];
	return self;
}

-(void)lockFocus {
	[super lockFocus];
	if ([[self openGLContext] view] != self)
		[[self openGLContext] setView:self];
}

-(void)dealloc {
	[self stopAnimation];
	
	[openGLContext release];
	[pixelFormat release];
	
	[[NSNotificationCenter defaultCenter] removeObserver:self name:NSViewGlobalFrameDidChangeNotification object:self];
	[super dealloc];
}	



-(void)awakeFromNib {
	NSLog(@"GLView::awakeFromNib, window:%@",[self window]);
	[[self window] setAcceptsMouseMovedEvents:YES];
    
}




//-(void)goFullscreenOnRect:(NSRect)rect {
//	windowMode = OF_FULLSCREEN;
//	[self stopAnimation];
//	
//	savedWindowFrame = [[self window] frame];
//	if(savedWindowFrame.size.width == 0 || savedWindowFrame.size.height == 0) {
//		savedWindowFrame.size = NSMakeSize(1024, 768);
//	}
//	
//	SetSystemUIMode(kUIModeAllHidden, NULL);
//	[[self window] setFrame:rect display:YES animate:NO];
//	[[self window] setLevel:NSMainMenuWindowLevel+1];
//	
//	[self startAnimation];
//}


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
	NSString *characters = [theEvent characters];
	if ([characters length]) {
		unichar key = [characters characterAtIndex:0];
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
    // kCVPixelFormatType_32ARGB, kCVPixelFormatType_32BGRA, kCVPixelFormatType_422YpCbCr8
    return @{
             (NSString *)kCVPixelBufferOpenGLCompatibilityKey : [NSNumber numberWithBool:YES],
             (NSString *)kCVPixelBufferPixelFormatTypeKey     : [NSNumber numberWithInt:kCVPixelFormatType_32ARGB]
			 //[NSNumber numberWithInt:kCVPixelFormatType_422YpCbCr8]
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
                    
                    
                    dict = CFDictionaryCreateMutable (
                            kCFAllocatorDefault,
                            5,
                            &kCFTypeDictionaryKeyCallBacks,
                            &kCFTypeDictionaryValueCallBacks);
                    
                    CFDictionarySetValue(dict, kCVOpenGLBufferTarget, [NSNumber numberWithInt:GL_TEXTURE_2D]);
                    CFDictionarySetValue(dict, kCVOpenGLBufferInternalFormat, [NSNumber numberWithInt:GL_RGB]);
                    CFDictionarySetValue(dict, kCVOpenGLBufferMaximumMipmapLevel, [NSNumber numberWithInt:6]);
                    CFDictionarySetValue(dict, kCVOpenGLBufferWidth, [NSNumber numberWithInt:[self width]]);
                    CFDictionarySetValue(dict, kCVOpenGLBufferHeight, [NSNumber numberWithInt:[self height]]);
                    CFDictionarySetValue(dict, kCVBufferPropagatedAttachmentsKey , [NSNumber numberWithInt:kCVAttachmentMode_ShouldNotPropagate]);
                    
                    CVReturn err2 = CVOpenGLBufferPoolCreate(NULL, NULL, dict, &_buffer);


                    //(CFDictionaryRef)ctxAttributes, &_textureCache);
                    if (err2 != noErr) {
                        NSLog(@"Error at CVOpenGLTextureCacheCreate %d", err);
                    }
                    
//                    if (dict) CFRelease (dict);
                }
                
                
                
                [self.player play];

                
            }
            _bLoaded=YES;
        });
    }];
    
    
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
			//NSLog(@"Mouse down outside window: %@", NSStringFromPoint([mouseEvent locationInWindow]));
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
