/*
 *  SpoutVisual.h
 *
 *  Created by Imanol Gomez on 11/01/20.
 *
 */

#pragma once

#include "VideoFrameSharingVisual.h"
#include "ofxSpout.h"


//=========================== class SpoutVisual ==============================
//============================================================================
/** \class SpoutVisual SpoutVisual.h
 *	\brief Represents a spout visual
 *	\details Creates a Spout Receiver and renders the Spout Sender's output into a texture
 */

class SpoutVisual: public VideoFrameSharingVisual
{

public:

    //! Constructor
    SpoutVisual();
    
    //! Constructor
    SpoutVisual(const BasicVisual& visual, bool centred = false);

    //! Destructor
    virtual ~SpoutVisual();
    
    //! setup the spout visual
    virtual void setup(const string& serverName, const string& appName);

    //! Updates the spout visual
	virtual void update();

	//! Draws the spout visual
	virtual void draw();
    
    //! Set the width
    virtual void setWidth(float width, bool keepRatio=false);

    //! Set the height
    virtual void setHeight(float height, bool keepRatio=false);

    //! Get the original width
	virtual float getOriginalWidth()  {return m_receiver.getWidth();}
    
    //! Get the original height
	virtual float getOriginalHeight()  {return m_receiver.getHeight();}
    
protected:

    ofxSpout::Receiver m_receiver; ///< spout receiver
	ofTexture m_texture;
	bool		m_isReceived;

};


