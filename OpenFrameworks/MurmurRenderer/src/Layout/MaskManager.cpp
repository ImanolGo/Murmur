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
        mask->allocate(windowSettingsVector[i].getWidth(), windowSettingsVector[i].getHeight(), GL_RGB, 4);
        mask->begin();  ofClear(0); mask->end();
        m_masks[i] = mask;
        
        auto maskee =  make_shared<ofFbo>();
        maskee->allocate(windowSettingsVector[i].getWidth(), windowSettingsVector[i].getHeight(), GL_RGBA, 4);
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

	ofShader shader;

	string path = "shaders/shadersGL2/RectMask";
	if (ofIsGLProgrammableRenderer()) {
		path = "shaders/shadersGL3/RectMask";
	}

	shader.load(path);
	ofLogNotice() << "MaskManager::setMaskWindowFront -> " << path;
    
    ofRectangle rect = AppManager::getInstance().getLayoutManager().getFrontLayout();
    
    ofLogNotice() <<"MaskManager::setMaskWindowFront: Rect-> x = " << rect.getX() << ", w = " << rect.getWidth();
	ofLogNotice() << "MaskManager::setMaskWindowFront: Rect-> y = " << rect.getY() << ", h = " << rect.getHeight();
    

	glm::vec2 resolution(m_masks[windowIndex]->getWidth(), m_masks[windowIndex]->getHeight());
    
    
    m_masks[windowIndex]->begin();
		ofClear(0);
		shader.begin();
		shader.setUniform2f("iResolution", resolution);
		shader.setUniform3f("BottomLeft", rect.getTopLeft());
		shader.setUniform3f("TopRight", rect.getBottomRight());
		ofDrawRectangle(0, 0, resolution.x, resolution.y);
		shader.end();
    m_masks[windowIndex]->end();

}

void MaskManager::setMaskWindowTop()
{
    int windowIndex = 2;
    
    if(m_masks.find(windowIndex) == m_masks.end()){
        return;
    }
    
    ofLogNotice() <<"MaskManager::setup Top Mask ";
	ofShader shader;

	string path = "shaders/shadersGL2/CircleMask";
	if (ofIsGLProgrammableRenderer()) {
		path = "shaders/shadersGL3/CircleMask";
	}

	shader.load(path);
	ofLogNotice() << "MaskManager::setMaskWindowTop -> " << path;
    
    ImageVisual gradientMask = ImageVisual(ofPoint(m_masks[windowIndex]->getWidth()*0.5,m_masks[windowIndex]->getHeight()*0.5), "floor_mask", true );
    gradientMask.setHeight(m_masks[windowIndex]->getHeight(), true);

	glm::vec2 resolution(m_masks[windowIndex]->getWidth(), m_masks[windowIndex]->getHeight());

  
    m_masks[windowIndex]->begin();
        ofClear(0);
		shader.begin();
		shader.setUniform2f("iResolution", resolution);
		ofDrawRectangle(0, 0, resolution.x, resolution.y);
		shader.end();

        //gradientMask.draw();
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
    ofClear(0,255);
    ofEnableAlphaBlending();
    m_maskShader.begin();
    m_maskShader.setUniformTexture("imageMask", m_masks[windowIndex]->getTexture(), 1);
    
  
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






