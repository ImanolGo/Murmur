/*
 *  BirdsSwarmVisual.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 22/09/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "BasicVisual.h"
#include "VisualEffects.h"


//======================== class BirdsSwarmVisual ============================
//============================================================================
/** \class BirdsSwarmVisual BirdsSwarmVisual.h
 *	\brief Class managing the sonic boom particles
 *	\details It creates and destroyes the sonic boom visuals
 */


class BirdsSwarmVisual: public BasicVisual
 {
    
public:
    
    //! Constructor
    BirdsSwarmVisual();
    
    //! Destructor
    ~BirdsSwarmVisual();
     
     
public:
     
    float   m_speed;               ///< Speed of the birds
    float   m_size;                ///< Size of the swarm
    int     m_number;              ///< Number of the birds
    float   m_birdSize;            ///< Size of the swarm
    float   m_randomness;          ///< Size of the swarm

};



//==============================================================================
/** \class BirdsNumberEffect BirdsSwarmVisual.h
 *	\brief Effect for changing the number of birds
 *	\details Provides basic member attributes and methods
 *			 to provide birds number animation
 */
//==============================================================================

class BirdsNumberEffect: public VisualEffect
{
public:
    
    //! Constructor
    BirdsNumberEffect(ofPtr<BirdsSwarmVisual> visual, EasingFunction function = LINEAR, EasingType type = EASE_IN);
    
    //========================= Fade Interface ================================
    
    //! updates the fade visual if active
    virtual void update();
    
    
    //==============================================================================
    
    //! Sets the final number value and the duration of the animation
    virtual void setParameters(int endNumber, double animationTime);
    
    //! Sets the starting and final number value and the duration of the animation
    virtual void setParameters(int startNumber,int endNumber, double animationTime);
    
    
protected:
    
    int     m_number;           ///< current number value
    int     m_startNumber;		///< start number value
    int     m_endNumber;        ///< end number value
    
    ofPtr<BirdsSwarmVisual>   m_birdsVisual;
    
    
};



//==============================================================================
/** \class BirdsRandonmnessEffect BirdsSwarmVisual.h
 *	\brief Effect for changing the birds randomness
 *	\details Provides basic member attributes and methods
 *			 to provide birds randomness animation
 */
//==============================================================================

class BirdsRandonmnessEffect: public VisualEffect
{
public:
    
    //! Constructor
    BirdsRandonmnessEffect(ofPtr<BirdsSwarmVisual> visual, EasingFunction function = LINEAR, EasingType type = EASE_IN);
    
    //========================= Fade Interface ================================
    
    //! updates the fade visual if active
    virtual void update();
    
    
    //==============================================================================
    
    //! Sets the final randomness value and the duration of the animation
    virtual void setParameters(float endRand, double animationTime);
    
    //! Sets the starting and final randomness value and the duration of the animation
    virtual void setParameters(float startRand,float endRand, double animationTime);
    
    
protected:
    
    float     m_randomness;     ///< current randomness value
    float     m_startRand;      ///< start randomness value
    float     m_endRand;        ///< end randomness value
    
    ofPtr<BirdsSwarmVisual>   m_birdsVisual;
    
    
};


//==============================================================================
/** \class BirdsSpeedEffect BirdsSwarmVisual.h
 *	\brief Effect for changing the birds speed
 *	\details Provides basic member attributes and methods
 *			 to provide birds speed animation
 */
//==============================================================================

class BirdsSpeedEffect: public VisualEffect
{
public:
    
    //! Constructor
    BirdsSpeedEffect(ofPtr<BirdsSwarmVisual> visual, EasingFunction function = LINEAR, EasingType type = EASE_IN);
    
    //========================= Fade Interface ================================
    
    //! updates the fade visual if active
    virtual void update();
    
    
    //==============================================================================
    
    //! Sets the final speed value and the duration of the animation
    virtual void setParameters(float endSpeed, double animationTime);
    
    //! Sets the starting and final speed value and the duration of the animation
    virtual void setParameters(float startSpeed,float endSpeed, double animationTime);
    
    
protected:
    
    float     m_speed;          ///< current speed value
    float     m_startSpeed;     ///< start speed value
    float     m_endSpeed;       ///< end speed value
    
    ofPtr<BirdsSwarmVisual>   m_birdsVisual;
    
    
};





