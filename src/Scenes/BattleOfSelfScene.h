/*
 *  BattleOfSelfScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 30/06/15.
 *
 */

#pragma once


#include "FluidVisual.h"
#include "ofxScene.h"
#include "SonicBoomVisual.h"
#include "ofxWater.h"

class BattleOfSelfScene : public ofxScene {

public:
    
    //Contructor
    BattleOfSelfScene();
    
    //Destructor
    ~BattleOfSelfScene();

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
    
    void setupSonicBoom();
    
    void drawFluid();
    
    void updateSonicBoom();
    
    void updateFluid();
    
    void drawSonicBoom();

    
private:
    
    bool            m_initialized;
    
    ofShader                m_shader;
    FluidVisual             m_fluid;
    ofFbo                   m_fbo;
    SonicBoomVisual         m_sonicBoomVisual;
        
    ofRectangle             m_drawArea;

};

