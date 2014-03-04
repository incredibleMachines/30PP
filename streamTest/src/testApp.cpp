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
    
    filepaths[0]="UV_L_24fps_2.mov";
    filepaths[1]="LeftWall_UV_3.mov";
    filepaths[2]="LeftWall_UV_4.mov";
    filepaths[3]="LeftWall_UV_5.mov";
    filepaths[4]="LeftWall_UV_6.mov";
    
    play=0;
    load=0;
    bFirst=true;
    count=BUFFER_SIZE;
    bContentLoaded=false;
    contentTriggered=false;
    
    
    drawTex.allocate(2000,720,GL_RGB);
    
    
	for(int i=0;i<BUFFER_SIZE;i++){
        contentBuffer[i].video.setPixelFormat(OF_PIXELS_RGB);
        contentBuffer[i].video.setLoopState(OF_LOOP_NORMAL);
        contentBuffer[i].loaded=false;
        contentBuffer[i].setup=false;
        
    }
}

//--------------------------------------------------------------
void testApp::update(){
    if(contentTriggered==true){
        if(bContentLoaded==false){
            bool all=true;
            for(int i=0;i<BUFFER_SIZE;i++){
                if(contentBuffer[i].thread.bFinished==true){
                    contentBuffer[i].loaded=true;
                    contentBuffer[i].thread.bFinished=false;
                    cout<<contentBuffer[i].thread.video.getMoviePath()<<endl;
                    contentBuffer[i].thread.video.play();
                    cout<<"loaded"<<endl;
                }
                else if(contentBuffer[i].loaded==false){
                    all=false;
                }
            }
            if(all==true){
                cout<<"loaded ALL"<<endl;
                bContentLoaded=true;
            }
    }
    
    else{
        if(contentBuffer[play].loaded==true){
            contentBuffer[play].thread.lock();
            contentBuffer[play].thread.video.update();
            drawTex.loadData(contentBuffer[play].thread.video.getPixels(),2000,720,GL_RGB);
            contentBuffer[play].thread.unlock();
            }
    

    
    if(contentBuffer[play].thread.video.getCurrentFrame()==contentBuffer[play].thread.video.getTotalNumFrames()&&contentBuffer[play].thread.video.getCurrentFrame()!=0){
        play++;
        if(play>BUFFER_SIZE-1){
            play=0;
        }
        
        load=play-1;
        if(load<0){
            load=BUFFER_SIZE-1;
        }
        
        cout<<"switch"<<endl;
        contentBuffer[play].thread.video.setFrame(0);
        bFirst=false;
        bLoaded=false;
        cout<<"playing: "<< contentBuffer[play].thread.video.getMoviePath()<<endl;
    }
    
    if(bFirst==false){
        if(contentBuffer[play].thread.video.getCurrentFrame()>contentBuffer[play].thread.video.getTotalNumFrames()/2&&bLoaded==false){

                contentBuffer[load].thread.bRun=true;
                contentBuffer[load].thread.filepath=filepaths[count];
                count++;
                if (count>NUM_VIDS-1){
                    count=0;
                }
                bLoaded=true;
                timer=ofGetElapsedTimeMillis();
            
            
//            delete contentBuffer[load].video;
        }
        
        else if(contentBuffer[load].thread.bFinished==true){
            contentBuffer[load].thread.video.play();
            contentBuffer[load].thread.bFinished=false;
                contentBuffer[load].loaded=true;
            
            cout<<"loaded: "<<contentBuffer[load].video.getMoviePath()<<endl;
//            contentBuffer[load].thread.stop();
        }
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

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='0'){
        for(int i=0;i<BUFFER_SIZE;i++){
            if(contentBuffer[i].setup==false){
                contentBuffer[i].video.loadMovie(filepaths[i]);
                contentBuffer[i].thread.start();
                contentBuffer[i].thread.filepath=filepaths[i];
                contentBuffer[i].thread.bRun=true;
                contentTriggered=true;
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

//--------------------------------------------------------------
void testApp::exit(){
    for(int i=0;i<BUFFER_SIZE;i++){
        contentBuffer[i].thread.stop();
    }
}
