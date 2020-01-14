/*
 *  VideoFrameSharingVisual.h
 *
 *  Created by Imanol Gomez on 11/01/20.
 *
 */

#pragma once

#include "BasicVisual.h"


//=========================== class VideoFrameSharingVisual ==============================
//============================================================================
/** \class VideoFrameSharingVisual VideoFrameSharingVisual.h
 *	\brief Represents a video frame sharing visual base class
 *	\details It will be use to unify Syphon and Spout visuals 
 */

class VideoFrameSharingVisual: public BasicVisual
{

public:

    //! Constructor
    VideoFrameSharingVisual(): BasicVisual(), m_centred(false), m_resizer(1,1,1) {}
    
    //! Constructor
    VideoFrameSharingVisual(const BasicVisual& visual, bool centred = false): BasicVisual(visual), m_centred(centred), m_resizer(1,1,1) {}

    //! Destructor
    virtual ~VideoFrameSharingVisual() {}
    
    //! setup the video frame sharing visual
    virtual void setup(const string& serverName, const string& appName) {}

    //! Updates the video frame sharing visual
	virtual void update() {}

	//! Draws the video frame sharing visual
	virtual void draw() {}
    
    //! Sets the texture to be binded to the quad
    virtual void setCentred(bool centred) {m_centred = centred;}

    //! Set the width
    virtual void setWidth(float width, bool keepRatio=false) {}

    //! Set the height
    virtual void setHeight(float height, bool keepRatio=false) {}

    //! Get the original width
    virtual float getOriginalWidth()  {return 0.0f;}
    
    //! Get the original height
    virtual float getOriginalHeight()  {return 0.0f;}
    

protected:

    bool                m_centred;          ///< defines if the visual should be centred or not
    ofVec3f             m_resizer;          ///< it deales with the resize of the picture

};


