/*
 *  ContourManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 26/06/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "VisualEffects.h"
#include "BasicVisual.h"

//========================== class ContourManager ==============================
//============================================================================
/** \class ContourManager ContourManager.h
 *    \brief Class managing the contours from the cameras
 *    \details It reads the contours and puts them into a texture
 */


class ContourManager: public Manager
{
    
public:
    
    //! Constructor
    ContourManager();
    
    //! Destructor
    ~ContourManager();
    
    //! Setup the Contour Manager
    void setup();
    
    //! Update the Contour Manager
    void update();
    
    //! Draw the Contour Manager
    void draw();
    
    //! Draw the Contour Manager
    void draw(const ofRectangle& area);
    
    const ofFbo& getSource() const;
    
    void resetContours();
    
    void setContour(vector<float> contourPoints);
    
    void setOffset(ofVec2f & offset);
    
    void setScale(ofVec2f & scale);
    
    void setContourThickness(float & value);
    
    void setSmokeBrightness(float & value);
    
    void onSendFarClipping(int& value); //Send far clipping command
    
    void onSendCropBottom(int& value); //Send far crop bottom command
    
    void addContourEffect(float targetContourThickness, double duration, double startTime = 0);
    
    float getSmokeBrightness() const {return m_smokeBrightness;}
    
private:
    
    void setupFbo();
    
    void setupShader();
    
    void setupEffects();
    
private:
    
    typedef vector< ofPtr< ofPolyline> > ContourVector;
    
    ofVec2f             m_contourOffset;
    ofVec2f             m_contourScale;
    
    
    ofPtr<BasicVisual>  m_contourVisual;
    ofPtr<MoveVisual>   m_contourEffect;
    
    ContourVector       m_contours;
    
    ofFbo                m_contourFbo;
    
    bool                m_isFrameNew;
    
    ofShader            m_thickLineShader;
    float               m_smokeBrightness;
    
    
    float               m_contourThickness;
    
};


