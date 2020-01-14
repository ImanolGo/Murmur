/*
 *  SyphonVisual.h
 *
 *  Created by Imanol Gomez on 28/08/15.
 *
 */

#pragma once

#include "VideoFrameSharingVisual.h"
#include "ofxSyphon.h"


//=========================== class SyphonVisual ==============================
//============================================================================
/** \class SyphonVisual SyphonVisual.h
 *	\brief Represents a syphon visual
 *	\details Creates a Syphon Receiver and renders the Spout Sender's output into a texture
 */

class SyphonVisual: public VideoFrameSharingVisual
{

public:

    //! Constructor
    SyphonVisual();
    
    //! Constructor
    SyphonVisual(const BasicVisual& visual, bool centred = false);

    //! Destructor
    virtual ~SyphonVisual();
    
    //! setup the syphon visual
    virtual void setup(const string& serverName, const string& appName);

	//! Draws the syphon visual
	virtual void draw();

    //! Set the width
    virtual void setWidth(float width, bool keepRatio=false);

    //! Set the height
    virtual void setHeight(float height, bool keepRatio=false);

    //! Get the original width
    float getOriginalWidth()  {return m_client.getWidth();}
    
    //! Get the original height
    float getOriginalHeight()  {return m_client.getHeight();}
    


protected:

	ofxSyphonClient     m_client;           ///< syphon client

};


