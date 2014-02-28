#include "testApp.h"

// High Performance OS X Video Playback Example
//--------------------------------------------------------------
// This example shows how to use the OS X platform specific
// ofQTKitPlayer on its own without the cross platform
// ofVideoPlayer wrapper.  Apps you write in this way won't be
// cross platform, but can be faster for HD videos .
//--------------------------------------------------------------

void testApp::setup(){
	ofBackground(255, 255, 255);
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    filepaths[0]="LeftWall_UV_6.mov";
    filepaths[1]="LeftWall_UV_3.mov";
    filepaths[2]="LeftWall_UV_4.mov";
    filepaths[3]="LeftWall_UV_5.mov";
    filepaths[4]="LeftWall_UV_2.mov";
//    filepaths[4]="LeftWall_UV_6.mov";
    
    play=0;
    load=0;
    bFirst=true;
    count=BUFFER_SIZE-1;
    bContentLoaded=false;
    contentTriggered=false;
    
    
    drawTex.allocate(2000,720,GL_RGB);
    loopCount=0;
    
    
	for(int i=0;i<BUFFER_SIZE;i++){
        contentBuffer[i].video.setPixelFormat(OF_PIXELS_RGB);
        contentBuffer[i].video.setLoopState(OF_LOOP_NORMAL);
        contentBuffer[i].loaded=false;
        contentBuffer[i].setup=false;
        
    }
}

//--------------------------------------------------------------
void testApp::update(){
    
        if(contentBuffer[play].loaded==true){
            contentBuffer[play].video.update();
            drawTex.loadData(contentBuffer[play].video.getPixels(),2000,720,GL_RGB);
            }
    

    
    if(contentBuffer[play].video.getCurrentFrame()==contentBuffer[play].video.getTotalNumFrames()&&contentBuffer[play].video.getCurrentFrame()!=0){
        
        play++;
        if(play>BUFFER_SIZE-1){
            play=0;
        }
        
        destroy=play-1;
        if(destroy<0){
            destroy=BUFFER_SIZE-1;
        }
        
        load=destroy-1;
        if(load<0){
            load=BUFFER_SIZE-1;
        }
        
        
        cout<<"switch"<<endl;
        contentBuffer[play].video.setFrame(0);
        bFirst=false;
        bLoaded=false;
        cout<<"playing: "<< contentBuffer[play].video.getMoviePath()<<endl;
    }
    
    if(bFirst==false){
        if(contentBuffer[play].video.getCurrentFrame()>contentBuffer[play].video.getTotalNumFrames()/2&&bLoaded==false){

                count++;
                if (count>NUM_VIDS-1){
                    count=0;
                }
                bLoaded=true;
            
            contentBuffer[destroy].video.stop();
            cout<<"stopped"<<endl;
            contentBuffer[destroy].video.close();
            cout<<"closed"<<endl;
            contentBuffer[load].video.loadMovie(filepaths[count]);
            contentBuffer[load].loaded=false;
            cout<<"Loading: "<<contentBuffer[load].video.getMoviePath()<<endl;
            loopCount++;
            
        }
        
    }
    
    
    for(int i=0;i<BUFFER_SIZE;i++){
        if(contentBuffer[i].loaded==false){
            if(contentBuffer[i].video.isAsyncLoaded()){
                cout<<"loaded"<<endl;
                contentBuffer[i].loaded=true;
                cout<<sizeof(contentBuffer[i].video.getPixels())<<endl;
                contentBuffer[i].video.play();
            }
        }
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(255,255,255);
    if(contentBuffer[play].loaded==true){
        drawTex.draw(0,0);
    }
    
    ofSetColor(0,0,0);
    ofDrawBitmapString("number loads: "+ofToString(loopCount)+"  seconds :"+ofToString(ofGetElapsedTimeMillis()/1000),200,800);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='0'){
        for(int i=0;i<BUFFER_SIZE;i++){
            if(contentBuffer[i].setup==false){
                contentBuffer[i].video.loadMovie(filepaths[i]);
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

