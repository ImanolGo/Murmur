/*
 *  BirdsAndPaperSceneFloor.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 15/10/15.
 *
 */

#pragma once

#include "FluidVisual.h"
#include "ofxScene.h"
#include "ImageVisual.h"

class BirdsAndPaperSceneFloor : public ofxScene {

public:
    
    //Contructor
    BirdsAndPaperSceneFloor();
    
    //Destructor
    ~BirdsAndPaperSceneFloor();

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
    
    void setupFbos();
    
    void setupShaders();
    
    void drawFluid();
    
    void updateFluid();
    
private:
    
    FluidVisual     m_fluid;
    bool            m_initialized;
    ofShader        m_maskShader;
    ofFbo           m_fboMask;
    ofFbo           m_fboFluid;
    ofRectangle     m_drawArea;

};

