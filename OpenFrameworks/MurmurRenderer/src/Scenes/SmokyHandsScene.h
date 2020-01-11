/*
 *  SmokyHandsScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 25/06/15.
 *
 */

#pragma once


#include "ofxScene.h"
#include "FluidVisual.h"

class SmokyHandsScene : public ofxScene {

public:
    
    //Contructor
    SmokyHandsScene();
    
    //Destructor
    ~SmokyHandsScene();

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
    
    void drawFluid();
    
    void updateFluid();
    
private:
    
    FluidVisual     m_smokeFluid;
    bool            m_initialized;
    ofRectangle     m_drawArea;

};

