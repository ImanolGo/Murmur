/*
 *  FloorManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 02/07/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"

//========================== class FloorManager ==============================
//============================================================================
/** \class FloorManager FloorManager.h
 *    \brief Class managing the position of the dancer
 *    \details It reads the position from top camera and calibrates it
 */


class FloorManager: public Manager
{
    
    static const int FLOOR_POS_CIRCLE_RADIUS;
    
public:
    
    //! Constructor
    FloorManager();
    
    //! Destructor
    ~FloorManager();
    
    //! Setup the Contour Manager
    void setup();
    
    //! Draw the Contour Manager
    void draw();
    
    //! Draw the Contour Manager
    void draw(const ofRectangle& area);
    
    const ofVec2f& getPosition() const {return m_floorPosition;}
    
    void setPosition(const ofVec2f& position);
    
    void setOffset(ofVec2f & offset);
    
    void setScale(ofVec2f & scale);
    
    void onInvertedCoordinates(bool& value) {m_invertedCoordinates = value;}
    
    void onMaskOn(bool& value);
    
    void onStretchMask(bool& value);
    
    void onSetKathakMinSize(float& value) {m_kathakMinSize = value;}
    
    void onSetKathakMaxSize(float& value) {m_kathakMaxSize = value;}
    
    void onSetKathakLineWidth(float& value) {m_kathakLineWidth = value;}
    
    float getKathakMinSize() const {return m_kathakMinSize;}
    
    float getKathakMaxSize() const {return m_kathakMaxSize;}
    
    float getKathakLineWidth() const {return m_kathakLineWidth;}
    
    
private:
    
    typedef vector< ofPtr<ofPath> > ContourVector;
    
    
    ofVec2f             m_floorPosition;
    ofVec2f             m_floorOffset;
    ofVec2f             m_floorScale;
    float               m_kathakMinSize;
    float               m_kathakMaxSize;
    float               m_kathakLineWidth;
    bool                m_invertedCoordinates;
    
};



