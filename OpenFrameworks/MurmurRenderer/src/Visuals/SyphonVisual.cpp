/*
 *  SyphonVisual.cpp
 *
 *  Created by Imanol Gomez on 28/08/15.
 *
 */


#include "AppManager.h"
#include "ResourceManager.h"

#include "SyphonVisual.h"


SyphonVisual::SyphonVisual(): VideoFrameSharingVisual()
{
    //Intentionally left empty
}


SyphonVisual::SyphonVisual(const BasicVisual& visual, bool centred): VideoFrameSharingVisual(visual, centred)
{
    //Intentionally left empty 
}

SyphonVisual::~SyphonVisual()
{
    //Intentionally left empty
}

void SyphonVisual::setup(const string& serverName, const string& appName)
{
    m_client.setup();
    m_client.set(serverName, appName);
    
    //m_width = m_client.getWidth();
    //m_height = m_client.getHeight();
    //m_resizer.x = m_width/m_client.getWidth();
    //m_resizer.y = m_height/m_client.getHeight();
    
    ofLogNotice() << "SyphonVisual::syphon width " << m_client.getWidth();
    
    ofLogNotice() << "SyphonVisual::syphon height " << m_client.getHeight();
}


void SyphonVisual::draw()
{

    ofPushMatrix();

        ofTranslate(m_position);

        ofScale(m_scale.x, m_scale.y);

        if(m_centred){
            ofTranslate(-m_width*0.5,-m_height*0.5);
        }

        ofRotateX(m_rotation.x);
        ofRotateY(m_rotation.y);
    
        m_resizer.x = m_width/m_client.getWidth();
        m_resizer.y = m_height/m_client.getHeight();
        ofScale(m_resizer.x,m_resizer.y);
    
        ofSetColor(m_color);
        m_client.draw(0, 0);

    ofPopMatrix();
}

void SyphonVisual::setWidth(float width, bool keepRatio)
{
    m_width = width;
    if(keepRatio){
        float ratio = m_client.getWidth()/m_client.getHeight();
        m_height = m_width/ratio;
    }
    
}

void SyphonVisual::setHeight(float height, bool keepRatio)
{
    m_height = height;
    if(keepRatio){
        float ratio = m_client.getWidth()/m_client.getHeight();
        m_width = m_height*ratio;
    }
}
