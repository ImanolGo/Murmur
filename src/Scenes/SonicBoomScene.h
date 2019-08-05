/*
 *  SonicBoomScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 21/03/16.
 *
 */

#pragma once


#include "ofxScene.h"
#include "SonicBoomVisual.h"

class SonicBoomScene : public ofxScene {

public:

    //Contructor
    SonicBoomScene();

    //Destructor
    ~SonicBoomScene();

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

    void setupFbo();

    void setupSonicBoom();

    void updateSonicBoom();
    
    void updateFbo();
    
    void setupShaders();

    void drawSonicBoom();


private:

    bool            m_initialized;

    ofShader                m_shader;
    ofFbo                   m_fbo;
    SonicBoomVisual         m_sonicBoomVisual;

    ofRectangle             m_drawArea;

};
