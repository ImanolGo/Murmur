/*
 *  BeautifulMindSceneFloorFloor.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 14/10/15.
 *
 */

#pragma once

#include "VideoVisual.h"

#include "ofxScene.h"



class BeautifulMindSceneFloor : public ofxScene {
    
public:

    //! Constructor
    BeautifulMindSceneFloor();
    
    //! Destructor
    ~BeautifulMindSceneFloor();

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
    
    //! Setup video
    void   setupVideo();
    
    //! Update Video
    void   updateVideo();
    
    //! Draw the complete scene
    void   drawVideo();
    
    
private:
    
     bool          m_initialized;
     VideoVisual   m_video;

};

