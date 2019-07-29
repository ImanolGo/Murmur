/*
 *  BirdsOpeningScene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 22/09/15.
 *
 */

#pragma once


#include "ofxScene.h"

class BirdsOpeningScene : public ofxScene {

public:

    //Contructor
    BirdsOpeningScene();
    
    //Destructor
    ~BirdsOpeningScene();
    
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
    
    void setInitialParameters();
    
    void startBirds();
    
private:
    
    float m_swarmSize;
    
     BasicVisual         m_visual;   ///< Visual representing position and size of the swarm. Used for animations
    bool    m_initialized;

};

