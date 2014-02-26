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
    
    filepaths[0]="LeftWall_UV_2.mov";
    filepaths[1]="LeftWall_UV_3.mov";
    filepaths[2]="LeftWall_UV_4.mov";
    filepaths[3]="LeftWall_UV_5.mov";
    filepaths[4]="LeftWall_UV_6.mov";
    
    play=0;
    load=0;
    bFirst=true;
    count=BUFFER_SIZE;
    
            drawTex.allocate(2000,720,GL_RGB);
    
    
	for(int i=0;i<BUFFER_SIZE;i++){
        contentBuffer.push_back(new videoLoader());
        contentBuffer[i]->video=new ofVideoPlayer();
        contentBuffer[i]->video->setPixelFormat(OF_PIXELS_RGB);
        contentBuffer[i]->video->setLoopState(OF_LOOP_NORMAL);
        contentBuffer[i]->loaded=false;
        contentBuffer[i]->setup=false;
        
    }
}

//--------------------------------------------------------------
void testApp::update(){

    
    if(contentBuffer[play]->loaded==true){
        contentBuffer[play]->video->update();
        drawTex.loadData(contentBuffer[play]->video->getPixels(),2000,720,GL_RGB);
        if(contentBuffer[play]->video->isFrameNew()){
//            cout<<contentBuffer[play]->video->getCurrentFrame()<<endl;
        }
    }
    
    if(contentBuffer[play]->video->getCurrentFrame()==contentBuffer[play]->video->getTotalNumFrames()&&contentBuffer[play]->video->getCurrentFrame()!=0){
        play++;
        if(play>BUFFER_SIZE-1){
            play=0;
        }
        
        load=play-1;
        if(load<0){
            load=BUFFER_SIZE-1;
        }
        
        cout<<"switch"<<endl;
        contentBuffer[play]->video->setFrame(0);
        bFirst=false;
        bLoaded=false;
        cout<<"playing: "<< contentBuffer[play]->video->getMoviePath()<<endl;
    }
    
    if(bFirst==false){
        if(contentBuffer[play]->video->getCurrentFrame()>contentBuffer[play]->video->getTotalNumFrames()/2&&bLoaded==false){
            contentBuffer[load]->thread.start();
            contentBuffer[load]->thread.bRun=true;
            contentBuffer[load]->thread.filepath=filepaths[count];
            count++;
            if (count>NUM_VIDS-1){
                count=0;
            }
            bLoaded=true;
            
//            delete contentBuffer[load]->video;
        }
        
        else if(contentBuffer[load]->thread.bFinished==true){
//            contentBuffer[load]->video->stop();
//            contentBuffer[load]->video->close();
//            contentBuffer[load]->video=new ofVideoPlayer();
            contentBuffer[load]->thread.lock();
//            contentBuffer[load]->video->setPixelFormat(OF_PIXELS_RGB);
            contentBuffer[load]->video=contentBuffer[load]->thread.video;
            contentBuffer[load]->video->setLoopState(OF_LOOP_NORMAL);
            contentBuffer[load]->video->play();
            contentBuffer[load]->thread.bFinished=false;
            contentBuffer[load]->thread.unlock();
            cout<<"loaded: "<<contentBuffer[load]->video->getMoviePath()<<endl;
            contentBuffer[load]->thread.stop();
        }
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(255,255,255);
    if(contentBuffer[play]->loaded==true){
        drawTex.draw(0,0);
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='0'){
        for(int i=0;i<BUFFER_SIZE;i++){
            if(contentBuffer[i]->setup==false){
//                contentBuffer[i]->thread.start();
                contentBuffer[i]->video->loadMovie(filepaths[i]);
                contentBuffer[i]->loaded=true;
                contentBuffer[i]->video->play();
                contentBuffer[i]->setup=true;
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
        contentBuffer[i]->thread.stop();
    }
}
