/*
 *  ProjectorCalibrationScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 20/07/16.
 *
 */

#pragma once

#include "FluidVisual.h"
#include "ofxScene.h"
#include "ImageVisual.h"

class ProjectorCalibrationScene : public ofxScene {

    static const float LINE_THICKNESS;
    
public:
    
    //Contructor
    ProjectorCalibrationScene();
    
    //Destructor
    ~ProjectorCalibrationScene();

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
    
    void drawFbos();
    
    void drawTestPattern();
    
    
private:
    
    bool            m_initialized;
    ofShader        m_thickLineShader;
    ofFbo           m_fboTestPattern;

};

