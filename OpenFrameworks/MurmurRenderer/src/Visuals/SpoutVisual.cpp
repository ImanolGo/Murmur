/*
 *  SpoutVisual.cpp
 *
 *  Created by Imanol Gomez on 11/01/20.
 *
 */


#include "SpoutVisual.h"


SpoutVisual::SpoutVisual(): VideoFrameSharingVisual(), m_isReceived(false)
{
    //Intentionally left empty
}


SpoutVisual::SpoutVisual(const BasicVisual& visual, bool centred): VideoFrameSharingVisual(visual, centred)
{
    //Intentionally left empty 
}

SpoutVisual::~SpoutVisual()
{
    //Intentionally left empty
}

void SpoutVisual::setup(const string& serverName, const string& appName)
{
    m_receiver.init(appName);
	m_texture.allocate(100, 100, GL_RGBA);
    
    //m_width = m_receiver.getWidth();
    //m_height = m_receiver.getHeight();
    //m_resizer.x = m_width/m_receiver.getWidth();
    //m_resizer.y = m_height/m_receiver.getHeight();
    
    ofLogNotice() << "SpoutVisual::setup-> spout width " << m_receiver.getWidth();
    
    ofLogNotice() << "SpoutVisual::setup-> spout height " << m_receiver.getHeight();
}

void SpoutVisual::update()
{
	m_isReceived = m_receiver.receive(m_texture);
}

void SpoutVisual::draw()
{
	if (!m_receiver.isInitialized() || !m_isReceived)
	{
        return;
    }

    ofPushMatrix();

        ofTranslate(m_position);

        ofScale(m_scale.x, m_scale.y);

        if(m_centred){
            ofTranslate(-m_width*0.5,-m_height*0.5);
        }

        ofRotateX(m_rotation.x);
        ofRotateY(m_rotation.y);
    
        m_resizer.x = m_width/m_receiver.getWidth();
        m_resizer.y = m_height/m_receiver.getHeight();
        ofScale(m_resizer.x,m_resizer.y);
    
        ofSetColor(m_color);
        m_texture.draw(0, 0);

    ofPopMatrix();
}

void SpoutVisual::setWidth(float width, bool keepRatio)
{
    m_width = width;
    if(keepRatio){
        float ratio = m_receiver.getWidth()/ m_receiver.getHeight();
        m_height = m_width/ratio;
    }
    
}

void SpoutVisual::setHeight(float height, bool keepRatio)
{
    m_height = height;
    if(keepRatio){
        float ratio = m_receiver.getWidth()/ m_receiver.getHeight();
        m_width = m_height*ratio;
    }
}
