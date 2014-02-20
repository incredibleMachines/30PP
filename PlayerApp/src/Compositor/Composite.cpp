/*
 Composite class. Combines ofVboMesh text, ofImage images, ofQTKitPlayer content to create a single ofFbo texture for binding to a single mesh in ModelMapper.
 
 methods
 - setup(int meshNum) creates texture with correct texture coordinates and inits all globals
 - update() draws texture into ofFbo object.
 - bind()/unbind() wrapper for ofFbo bind/unbind, called from ModelMapper
 - drawFbo() draw all objects from SceneContent into Fbo in a appropriate positions determine by their scene and zone type
 - loadScene load new SceneContent data
 */

#include "Composite.h"

//--------------------------------------------------------------
void Composite::setup(int _meshNum){
    
    //----------TEXTURE SETUP
    
    //setup mipmapping GL context
    ofEnableAntiAliasing();
    ofDisableArbTex();
    
    //assign this texture to a specific mesh in ModelMapper
    meshNum=_meshNum;
    
    //set status variables
    bFinished=true;
    bLoaded=false;
    bPlaying=false;
    textPos=ofVec2f(0,200);
    
    //load our non-MipMapper texture for comparison
    ofLoadImage(drawNoMip, "mapping_test/mesh_"+ofToString(meshNum)+".jpg");
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    //load pixel data to set our mipmapper texture
    ofPixels pix;
	bool loaded = ofLoadImage(pix, "mapping_test/mesh_"+ofToString(meshNum)+".jpg");
    
    //allocate texture
    drawSurface.allocate(pix.getWidth(), pix.getHeight(), ofGetGlInternalFormat(pix) );

    ofTextureData& texData = drawSurface.texData;
    
    //save format and type as globals
    glFormat=ofGetGlFormat(pix);
    glType=ofGetGlType(pix);
    
    //special case for texture type - we will always be using GL_2D but just in case
    if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB){
        texData.tex_t = pix.getWidth();
        texData.tex_u = pix.getHeight();
    } else {
        texData.tex_t = (float)(pix.getWidth()) / (float)texData.tex_w;
        texData.tex_u = (float)(pix.getHeight()) / (float)texData.tex_h;
    }
    
    //set our pixel source to determine mip map texel size
    ofSetPixelStorei(pix.getWidth(),pix.getBytesPerChannel(),pix.getNumChannels());
    
    //create texture
    glGenTextures(1, &texData.textureID);
    
    //bind texture
    glBindTexture(texData.textureTarget, texData.textureID);
    
    //setup mipmaps
    glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, pix.getWidth(), pix.getHeight(), 0, ofGetGlFormat(pix), ofGetGlType(pix), pix.getPixels());
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    
    //set environment, not using currenty but just incase we change our env elsewhere
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //enable GL_TEXTURE_2D
    glEnable(  texData.textureTarget);
    
    //create mipmaps
    glGenerateMipmap(texData.textureTarget);
    
    //set params, GL_LINEAR_MIPMAP_LINER is trilinear which means greatest quality
    glTexParameteri( texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri( texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    //anisotropy unblurs our mip maps, basically sets "how much" we want antialias with higher param being less blur
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    glDisable( texData.textureTarget );
}

void Composite::update(){
    
    //--------UPDATE VIDEO TEXTURES
    
    //TODO make video textures load correctly, switch them to ofQTKitPlayer
    
    if(bPlaying==false&&bLoaded==true){
        if(currentScene->vid.size()!=0){
            for(int i=0; i<currentScene->vid.size();i++){
                currentScene->vid[i].play();
            }
        }
        bPlaying=true;
    }
    if(bPlaying==true){
        
        for(int i=0; i<currentScene->vid.size();i++){
            currentScene->vid[i].update();
            if(currentScene->vid[i].isFrameNew()){
                    createTexture();
            }
        }
    }
    
    pos.x=0;
    pos.y=0;
    

}

void Composite::bind(){
    drawSurface.bind();
    
}

void Composite::unbind(){
    drawSurface.unbind();
    
}

void Composite::createTexture(){
    
    
    //----------CREATE TEXTURE

    ofSetColor(255,255,255);




    //check to make sure content has loaded before drawing
    if(bLoaded==true){
        
        //save current frame pixel data to a pixel array for loading into glTexImage2D
        unsigned char * pix=currentScene->vid[0].getPixels();
        
        //set other glTexImage2D variables
        int width=currentScene->vid[0].getWidth();
        int height=currentScene->vid[0].getHeight();
        int pixelFormat=currentScene->vid[0].getPixelFormat();
    
        //pair with current texture
        ofTextureData& texData = drawSurface.texData;
        
        //bind texture
        glBindTexture(texData.textureTarget, texData.textureID);
        
        //setup mipmap context
        glTexImage2D(texData.textureTarget, 0, glFormat, drawSurface.getWidth(), drawSurface.getHeight(), 0, glFormat, glType, pix);
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        
        //environment, in case we change elsewhere
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        //enable GL_TEXTURE_2D
        glEnable(  texData.textureTarget);
        
        //make mipmaps - REQUIRES GL to be post-3.0
        glGenerateMipmap(texData.textureTarget);
        
        //set quality - GL_LINEAR_MIPMAP_LINEAR is highest, trilinear
        glTexParameteri( texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri( texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        //set amount of anisotropic filtering - this undoes the blur of the mipmapping relative to camera
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
        glDisable( texData.textureTarget );
    }
}

void Composite::loadScene(SceneContent::meshScene * _currentScene){
    
    //reference meshScene for this scene, this mesh - passed by playerApp
    currentScene = _currentScene;
    bLoaded=true;
    bPlaying=false;
}