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



MaskManager::MaskManager(): Manager(),  m_showMasks(false)
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
    
    this->setupShader();
    this->setupMasks();
    
    ofLogNotice() <<"MaskManager::initialized";
   
}

void MaskManager::setupShader()
{
    string path = "shaders/shadersGL2/BlackMask";
    if(ofIsGLProgrammableRenderer()){
        path = "shaders/shadersGL3/BlackMask";
    }
    
    m_maskShader.load(path);
    ofLogNotice() <<"MaskManager::setupShader -> " << path;
}


void MaskManager::setupMasks()
{
    
    auto windowSettingsVector = WindowSettingsManager::getInstance().getWindowsSettings();
    
    for(int i=0; i<windowSettingsVector.size(); i++)
    {
        auto mask =  make_shared<ofFbo>();
        mask->allocate(windowSettingsVector[i].getWidth(), windowSettingsVector[i].getHeight(), GL_RGBA);
        mask->begin();  ofClear(0); mask->end();
        m_masks[i] = mask;
        
        auto maskee =  make_shared<ofFbo>();
        maskee->allocate(windowSettingsVector[i].getWidth(), windowSettingsVector[i].getHeight(), GL_RGBA);
        maskee->begin();  ofClear(0); maskee->end();
        m_masksees[i] = maskee;
        
        ofLogNotice() <<"MaskManager::setupMasks -> id = " << i << ", width =  " << windowSettingsVector[i].getWidth() << ", height =  " << windowSettingsVector[i].getHeight();
    }
    

    this->setMaskWindowFront();
    this->setMaskWindowTop();
}

void MaskManager::setMaskWindowFront()
{
    int windowIndex = 1;
    
    if(m_masks.find(windowIndex) == m_masks.end()){
        return;
    }
    
    ofLogNotice() <<"MaskManager::setup Front Mask ";
    
    ofRectangle rect = getFrontMaskRectangle();
    
    ofLogNotice() <<"MaskManager::Rect-> x = " << rect.getX() << ", w = " << rect.getWidth();
    
    ImageVisual gradientMask = ImageVisual(ofPoint(0,0), "frame_mask" );
    //gradientMask.setWidth(m_masks[windowIndex]->getWidth()); gradientMask.setHeight(m_masks[windowIndex]->getHeight());
    gradientMask.setWidth(rect.getWidth()); gradientMask.setHeight(rect.getHeight());
    gradientMask.setPosition(ofPoint(rect.getX(),rect.getY()));
    
    
    m_masks[windowIndex]->begin();
        ofClear(0, 0, 0);
        gradientMask.draw();
    m_masks[windowIndex]->end();

}

void MaskManager::setMaskWindowTop()
{
    int windowIndex = 2;
    
    if(m_masks.find(windowIndex) == m_masks.end()){
        return;
    }
    
    ofLogNotice() <<"MaskManager::setup Top Mask ";
    
    ImageVisual gradientMask = ImageVisual(ofPoint(m_masks[windowIndex]->getWidth()*0.5,m_masks[windowIndex]->getHeight()*0.5), "floor_mask", true );
    gradientMask.setHeight(m_masks[windowIndex]->getHeight(), true);

  
    m_masks[windowIndex]->begin();
        ofClear(0, 0, 0);
        gradientMask.draw();
    m_masks[windowIndex]->end();
   
}

void MaskManager::begin(int windowIndex)
{
    if(m_masks.find(windowIndex) == m_masks.end()){
        return;
    }
    
    if(m_masksees.find(windowIndex) == m_masksees.end()){
        return;
    }
    
    
    m_masksees[windowIndex]->begin();
    ofEnableAlphaBlending();
    m_maskShader.begin();
    m_maskShader.setUniformTexture("imageMask", m_masks[windowIndex]->getTexture(), 1);
    
    ofClear(0);
}

void MaskManager::end(int windowIndex)
{
    if(m_masksees.find(windowIndex) == m_masksees.end()){
        return;
    }
    
    m_maskShader.end();
    m_masksees[windowIndex]->end();
    
    m_masksees[windowIndex]->draw(0,0);
    //m_masks[windowIndex]->draw(0,0);
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



