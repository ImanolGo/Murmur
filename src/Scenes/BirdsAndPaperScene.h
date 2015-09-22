/*
 *  BirdsAndPaperScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 22/09/15.
 *
 */

#pragma once


#include "SyphonVisual.h"
#include "ofxScene.h"

class BirdsAndPaperScene : public ofxScene {

public:

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

};

