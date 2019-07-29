/*
 *  PostShowScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 21/10/15.
 *
 */

#pragma once


#include "ofxScene.h"

#include "ImageVisual.h"

class PostShowScene : public ofxScene {

public:

    //Contructor
    PostShowScene();
    
    //Destructor
    ~PostShowScene();
    
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
    
    void setupImages();
    
    void drawImages();
    
    //! Calculate the drawing area
    ofRectangle  getDrawingArea();
    
private:
    
    ImageVisual     m_image;
    bool            m_initialized;

};

