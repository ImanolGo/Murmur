/*
 *  SonicBoomVisual.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 31/07/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "BasicVisual.h"
#include "ImageVisual.h"



//========================== class SonicBoomParticle ==============================
//============================================================================
/** \class SonicBoomParticle SonicBoomVisual.h
 *	\brief Sonic Boom Particle
 */



class SonicBoomParticle: public BasicVisual
{
    
public:
    
    
    //! Constructor
    SonicBoomParticle(const ofPoint& pos);
    
    //! Destructor
    ~SonicBoomParticle();
    
    //! Setup the Sonic boom particle
    void setup();
    
    //! Update the Sonic boom particle
    void update();
    
    //! Draw the Sonic boom particle
    void draw();
    
    bool isAlive() {return m_live;}

    
private:
    
    float m_initSize;            //Initial size of the particle
    float m_time;                //Time of living
    float m_lifeTime;            //Allowed lifetime
    float m_size;                ///Max size
    bool  m_live;                //Is particle live
    
    ImageVisual  m_image;
};


//========================== class SonicBoomVisual ==============================
//============================================================================
/** \class SonicBoomVisual SonicBoomVisual.h
 *	\brief Class managing the sonic boom particles
 *	\details It creates and destroyes the sonic boom visuals
 */


class SonicBoomVisual
 {
    
public:
    
    //! Constructor
    SonicBoomVisual();
    
    //! Destructor
    ~SonicBoomVisual();
     
    //! set up the Sonic boom visual
    void setup();
    
    //! Draw the Sonic boom visual
    void update();
     
    //! Draw the Sonic boom visual
    void draw();
     
     //! Clear the Sonic boom visual
    void clear();
     
    const ofFbo& getFbo() const {return m_fbo;}
     
    void addParticle(const ofPoint& pos);
     
    bool empty() {return m_particles.empty();}
     
private:
     
     void updateParticles();
     
     void updateFbo();
     
     void drawParticles();
     
private:
     
     typedef vector< ofPtr<SonicBoomParticle> > ParticlesVector;
     ParticlesVector  m_particles;
     
     ofFbo                  m_fbo;
     
     double                 m_elapsedTime;
     double                 m_newParticleTime;

};



