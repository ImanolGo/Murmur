/*
 *  KathakScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 30/07/15.
 *
 */

#pragma once

#include "ofxScene.h"
#include "ImageVisual.h"
#include "ofxWater.h"

class KathakScene : public ofxScene {

public:
    
    //Contructor
    KathakScene();
    
    //Destructor
    ~KathakScene();

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
    
    void setupWaterRipples();
    
    void setupFbos();
    
    void setupShaders();
    
    void drawWaterRipples();
    
    void updateWaterRipples();
    
private:

    bool            m_initialized;
    ofShader        m_maskShader;
    ofFbo           m_fboMask;
    ofRectangle     m_drawArea;
    ofxWater        m_water;

};

