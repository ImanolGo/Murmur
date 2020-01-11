/*
 *  BeautifulMindManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 04/07/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "SettingsManager.h"



struct BeautifulMindTimingSettings
{
    int id;
    float duration;
    
    BeautifulMindTimingSettings(): id(0), duration(0.0f){}
    
};

//========================== class BeautifulMindManager ==============================
//============================================================================
/** \class BeautifulMindManager BeautifulMindManager.h
 *	\brief Class managing the beautiful mind scene parameters
 *	\details It reads the parameters sets the FBO of the Beautiful Mind Scene
 */


class BeautifulMindManager: public Manager
 {

public:
    
    //! Constructor
    BeautifulMindManager();
    
    //! Destructor
    ~BeautifulMindManager();
    
    //! Setup the Hands Manager
    void setup();
    
    //! Draw the Hands Manager
    void draw();
     
    void setOffset(ofVec2f & offset);
     
    void setScale(ofVec2f & scale);
     
    const ofRectangle& getRectangleSpace() const {return m_rectangleSpace;}
    
    float getTiming(int id) const;
     
    void setFloorSpeed(float &speed ) {m_floorSpeed = speed;}
     
    void setFloorOn(bool &floorOn ) {m_floorOn = floorOn;}
     
    void setCalibrationOn(bool &calibrationOn ) {m_calibrationOn = calibrationOn;}
     
    float getFloorSpeed() const {return m_floorSpeed;}
     
    bool isFloorOn() const {return m_floorOn;}
     
    bool isCalibrationOn() const {return m_calibrationOn;}

     
private:
     
     void setupRectangleSpace();
     
     void setupTiming();
     
    
private:
     
    ofVec2f             m_handsOffset;
    ofVec2f             m_handsScale;
    ofRectangle         m_rectangleSpace;
    
    bool                m_floorOn;
    bool                m_calibrationOn;
    float               m_floorSpeed;
     
};



