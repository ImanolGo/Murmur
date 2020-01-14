/*
 *  BeautifulMindScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 02/07/15.
 *
 */

#pragma once

#include "ImageVisual.h"
#include "VideoVisual.h"

#include "ofxScene.h"


class BeautifulMindScene : public ofxScene {


public:

    //! Constructor
    BeautifulMindScene();
    
    //! Destructor
    ~BeautifulMindScene();

    //! Set up the scene
    void setup();

    //! Update the scene
    void update();

    //! Draw the scene
    void draw();
    
    //! Called when fading in
    void willFadeIn();

    //! Called when to start drawing
    void willDraw();

    //! Called fading out
    void willFadeOut();

    //! Called when exit
    void willExit();
    
private:
    
    //! Setup Fbo
    void   setupFbos();
    
    //! Setup mask
    void  setupMask();
    
    //! Setup shader
    void  setupShader();
    
    //! Setup video
    void setupVideo();
    
    //! Setup images
    void setupImages();
    
    //! update video
    void  updateVideo();
    
    //! update video
    void  updateMask();
    
    //! update video
    void   drawVideo();
    
    //! draw video
    void   drawScene();
    
    //! draw calibration
    void   drawCalibration();
    
    //! Calculate the drawing area
    ofRectangle  getDrawingArea();
    
   private:
    
     typedef   map < string, ofPtr< ImageVisual > >  ImageMap;
     ImageMap      m_images;
    
     bool          m_initialized;
	 ofVideoPlayer   m_video;
    
     ofFbo           m_fboVideo;

	 ofShader        m_maskShader;
     ofFbo           m_maskFbo;

};

