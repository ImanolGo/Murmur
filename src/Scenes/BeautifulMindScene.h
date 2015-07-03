/*
 *  BeautifulMindScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 02/07/15.
 *
 */

#pragma once


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
    
     bool            m_initialized;

};
