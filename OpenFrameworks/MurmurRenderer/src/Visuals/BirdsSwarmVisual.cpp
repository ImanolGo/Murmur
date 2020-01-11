/*
 *  BirdsSwarmVisual.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 22/09/15.
 *
 */

#include "BirdsSwarmVisual.h"


BirdsSwarmVisual::BirdsSwarmVisual(): BasicVisual()
{
    //Intentionaly left empty
}


BirdsSwarmVisual::~BirdsSwarmVisual()
{
    //Intentionaly left empty
}


//==============================================================================
//=========================== BirdsNumberEffect ================================
//==============================================================================

BirdsNumberEffect::BirdsNumberEffect(ofPtr<BirdsSwarmVisual> visual, EasingFunction function, EasingType type): VisualEffect(visual,function,type), m_number(0), m_startNumber(0), m_endNumber(0)
{
    m_birdsVisual = visual;
    m_name = "BirdsNumberEffect";
}



void BirdsNumberEffect::setParameters(int startNumber,int endNumber, double animationTime)
{
    m_elapsedTime = 0.0;
    m_startNumber = startNumber;
    m_endNumber = endNumber;
    m_animationTime = animationTime;
}

void BirdsNumberEffect::setParameters(int endNumber, double animationTime)
{
    m_elapsedTime = 0.0;
    m_startNumber = m_birdsVisual->m_number;
    m_endNumber = endNumber;
    m_animationTime = animationTime;
}


void BirdsNumberEffect::update()
{
    if(!m_isActive){
        return;
    }
    
    double dt = ofGetLastFrameTime();
    
    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
        return;
    }
    
    m_elapsedTime = m_elapsedTime + dt;
    
    if(m_elapsedTime >= m_animationTime) {
        m_birdsVisual->m_number = m_endNumber;
        this->finish();
        return;
    }
    
    m_number = this->function(m_elapsedTime,m_startNumber,m_endNumber,m_animationTime);
    m_birdsVisual->m_number = m_number;
    
}




//==============================================================================
//======================== BirdsRandonmnessEffect ==============================
//==============================================================================

BirdsRandonmnessEffect::BirdsRandonmnessEffect(ofPtr<BirdsSwarmVisual> visual, EasingFunction function, EasingType type): VisualEffect(visual,function,type), m_randomness(0), m_startRand(0), m_endRand(0)
{
    m_birdsVisual = visual;
    m_name = "BirdsRandonmnessEffect";
}



void BirdsRandonmnessEffect::setParameters(float startRand,float endRand, double animationTime)
{
    m_elapsedTime = 0.0;
    m_startRand = startRand;
    m_endRand = endRand;
    m_animationTime = animationTime;
}

void BirdsRandonmnessEffect::setParameters(float endRand, double animationTime)
{
    m_elapsedTime = 0.0;
    m_randomness = m_birdsVisual->m_randomness;
    m_endRand = endRand;
    m_animationTime = animationTime;
}


void BirdsRandonmnessEffect::update()
{
    if(!m_isActive){
        return;
    }
    
    double dt = ofGetLastFrameTime();
    
    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
        return;
    }
    
    m_elapsedTime = m_elapsedTime + dt;
    
    if(m_elapsedTime >= m_animationTime) {
        m_birdsVisual->m_randomness = m_endRand;
        this->finish();
        return;
    }
    
    m_randomness = this->function(m_elapsedTime,m_startRand,m_endRand,m_animationTime);
    m_birdsVisual->m_randomness = m_randomness;
    
}


//==============================================================================
//============================== BirdsSpeedEffect ==============================
//==============================================================================

BirdsSpeedEffect::BirdsSpeedEffect(ofPtr<BirdsSwarmVisual> visual, EasingFunction function, EasingType type): VisualEffect(visual,function,type), m_speed(0), m_startSpeed(0), m_endSpeed(0)
{
    m_birdsVisual = visual;
    m_name = "BirdsSpeedEffect";
}



void BirdsSpeedEffect::setParameters(float startSpeed,float endSpeed, double animationTime)
{
    m_elapsedTime = 0.0;
    m_startSpeed = startSpeed;
    m_endSpeed = endSpeed;
    m_animationTime = animationTime;
}

void BirdsSpeedEffect::setParameters(float endSpeed, double animationTime)
{
    m_elapsedTime = 0.0;
    m_startSpeed = m_birdsVisual->m_speed;
    m_endSpeed = endSpeed;
    m_animationTime = animationTime;
}


void BirdsSpeedEffect::update()
{
    if(!m_isActive){
        return;
    }
    
    double dt = ofGetLastFrameTime();
    
    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
        return;
    }
    
    m_elapsedTime = m_elapsedTime + dt;
    
    if(m_elapsedTime >= m_animationTime) {
        m_birdsVisual->m_speed = m_endSpeed;
        this->finish();
        return;
    }
    
    m_speed = this->function(m_elapsedTime,m_startSpeed,m_endSpeed,m_animationTime);
    m_birdsVisual->m_speed = m_speed;
    
}
