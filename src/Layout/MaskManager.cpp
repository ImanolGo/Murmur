/*
 *  MaskManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 18/11/15.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "MaskManager.h"
#include "WindowSettingsManager.h"



MaskManager::MaskManager(): Manager()
{
	//Intentionally left empty
}


MaskManager::~MaskManager()
{
    ofLogNotice() <<"MaskManager::Destructor";
}


void MaskManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();
    
    this->setupMasks();
    
    ofLogNotice() <<"MaskManager::initialized";
   
}

void MaskManager::setupMasks()
{
    
    auto windowSettingsVector = WindowSettingsManager::getInstance().getWindowsSettings();
    
    for(auto windowSettings : windowSettingsVector)
    {
        ofPtr<ofxMask> mask =  ofPtr<ofxMask>(new ofxMask());
        mask->allocate(windowSettings.getWidth(), windowSettings.getHeight(), ofxMask::LUMINANCE);
    
        ofLogNotice() <<"MaskManager::setupMasks -> width =  " << windowSettings.getWidth() << ", height =  " << windowSettings.getHeight();
        m_masks.push_back(mask);
    }
    

    this->setMaskWindowFront();
    this->setMaskWindowTop();
}

void MaskManager::setMaskWindowFront()
{
    int windowIndex = 1;
    
    if(windowIndex<0 ||  windowIndex > (m_masks.size()-1)){
        return;
    }
    
    ofLogNotice() <<"MaskManager::setup Front Mask ";
    
    ofRectangle rect = getFrontMaskRectangle();
    
    ofLogNotice() <<"MaskManager::Rect-> x = " << rect.getX() << ", w = " << rect.getWidth();
    
    ImageVisual gradientMask = ImageVisual(ofPoint(0,0), "frame_mask" );
    //gradientMask.setWidth(m_masks[windowIndex]->getWidth()); gradientMask.setHeight(m_masks[windowIndex]->getHeight());
    gradientMask.setWidth(rect.getWidth()); gradientMask.setHeight(rect.getHeight());
    gradientMask.setPosition(ofPoint(rect.getX(),rect.getY()));
    
    
    m_masks[windowIndex]->beginMask();
        ofClear(0, 0, 0);
        gradientMask.draw();
    m_masks[windowIndex]->endMask();

}

void MaskManager::setMaskWindowTop()
{
    int windowIndex = 2;
    
    if(windowIndex<0 ||  windowIndex > (m_masks.size()-1)){
        return;
    }
    
    ofLogNotice() <<"MaskManager::setup Top Mask ";
    
    ImageVisual gradientMask = ImageVisual(ofPoint(m_masks[windowIndex]->getWidth()*0.5,m_masks[windowIndex]->getHeight()*0.5), "floor_mask", true );
    gradientMask.setHeight(m_masks[windowIndex]->getHeight(), true);

  
    m_masks[windowIndex]->beginMask();
        ofClear(0, 0, 0);
        gradientMask.draw();
    m_masks[windowIndex]->endMask();

    
}

void MaskManager::begin(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_masks.size()-1)){
        return;
    }
    
    ofEnableAlphaBlending();
    m_masks[windowIndex]->begin(true);
}

void MaskManager::end(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_masks.size()-1)){
        return;
    }
    
    
    //m_maskShader.end();
   m_masks[windowIndex]->end();
    
   //m_masks[windowIndex]->drawMasker();
   m_masks[windowIndex]->draw();
    
   ofDisableAlphaBlending();
   
}


ofRectangle MaskManager::getFrontMaskRectangle()
{
    int windowIndex = 1;
    
    if(windowIndex<0 ||  windowIndex > (m_masks.size()-1)){
        return;
    }
    
    LayoutManager& layoutManager = AppManager::getInstance().getLayoutManager();
    float x = layoutManager.getCropLeft();
    float y = layoutManager.getCropTop();
    float w = m_masks[windowIndex]->getWidth() - layoutManager.getCropRight() - x;
    float h = m_masks[windowIndex]->getHeight() - layoutManager.getCropBottom() - y;
    
    
    return ofRectangle(x, y, w, h);
}



