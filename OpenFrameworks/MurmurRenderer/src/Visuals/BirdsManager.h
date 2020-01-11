/*
 *  BirdsManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 05/08/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"

#include "SyphonVisual.h"
#include "BirdsSwarmVisual.h"
#include "VisualEffects.h"

//========================== class BirdsManager ==============================
//============================================================================
/** \class BirdsManager BirdsManager.h
 *	\brief Class managing the bird swarm created in unity
 *	\details It communicates the values to Unity through OSC and it gets the render window using Syphon
 */


class BirdsManager: public Manager
{
    
public:
    
    //! Constructor
    BirdsManager();
    
    //! Destructor
    ~BirdsManager();
    
    //! Setup the Birds Manager
    void setup();
    
    //! Update the Birds Manager
    void update();
    
    //! Draw the Birds Manager
    void draw();
    
    void onChangePosition(ofVec3f& target);
    
    void onChangeSize(float& value);
    
    void onChangeSpeed(float& value);
    
    //void onChangeSwarmSize(float& value);
    
    void onChangeSwarmSize(ofVec3f& size);
    
    void onChangeSwarmNumber(int& value);
    
    void onChangeSwarmRandomness(float& value);
    
    void addMoveEffect(const ofVec3f& targetPosition, double duration, double startTime = 0);
    
    void addScaleEffect(const ofVec3f& targetScale, double duration, double startTime = 0);
    
    void addNumberEffect(int number, double duration, double startTime = 0);
    
    void addRandomnessEffect(float randomness, double duration, double startTime = 0);
    
    void addSpeedEffect(float speed, double duration, double startTime = 0);
    
    void stopEffects();
    
private:
    
    void setupSyphon();
    
    void setupShader();
    
    void setupEffects();
    
    
private:
    
    ofPtr<BirdsSwarmVisual>             m_swarm;
    ofPtr<MoveVisual>                   m_moveEffect;
    ofPtr<ScaleVisual>                  m_scaleEffect;
    ofPtr<BirdsRandonmnessEffect>       m_randEffect;
    ofPtr<BirdsNumberEffect>            m_numberEffect;
    ofPtr<BirdsSpeedEffect>             m_speedEffect;
    
    SyphonVisual        m_syphonVisual;
    ofShader            m_shader;
};


