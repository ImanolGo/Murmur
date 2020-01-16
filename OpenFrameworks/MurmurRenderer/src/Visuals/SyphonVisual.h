/*
 *  SyphonVisual.h
 *
 *  Created by Imanol Gomez on 28/08/15.
 *
 */

#pragma once

#include "BasicVisual.h"
#include "ofxSyphon.h"


//=========================== class SyphonVisual ==============================
//============================================================================
/** \class SyphonVisual SyphonVisual.h
 *	\brief Represents a syphon visual
 *	\details Creates a Syphon Client and renders the Spython Server output into a texture
 */

class SyphonVisual: public BasicVisual
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
    
    //! Sets the texture to be binded to the quad
    virtual void setCentred(bool centred) {m_centred = centred;}

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
    bool                m_centred;          ///< defines if the visual should be centred or not
    ofVec3f             m_resizer;          ///< it deales with the resize of the picture

};


