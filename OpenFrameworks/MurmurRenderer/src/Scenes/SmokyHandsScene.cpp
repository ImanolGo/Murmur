/*
 *  SmokyHandsScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 25/06/15.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"

#include "SmokyHandsScene.h"


SmokyHandsScene::SmokyHandsScene():ofxScene("SmokyHandsScene"), m_initialized(false)
{
    
}

SmokyHandsScene::~SmokyHandsScene()
{
    //Intentionally left empty
}

void SmokyHandsScene::setup()
{
    if (m_initialized) {
        return;
    }
    
    ofLogNotice("SmokyHandsScene::setup");
    m_smokeFluid.setup("xmls/SmokyHandsFluid.xml");
    
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    m_drawArea = ofRectangle(0, 0, windowsSettings.getWidth(), windowsSettings.getHeight());
    
    m_initialized = true;
}

void SmokyHandsScene::update()
{
    this->updateFluid();
}


void SmokyHandsScene::draw() {
     ofClear(0);
    this->drawFluid();
}

void SmokyHandsScene::updateFluid()
{
    auto source = AppManager::getInstance().getContourManager().getSource();
    m_smokeFluid.setSource(source);
    m_smokeFluid.update();
}

void SmokyHandsScene::drawFluid()
{
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    
    //AppManager::getInstance().getContourManager().draw(m_drawArea);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    m_smokeFluid.draw(m_drawArea);
    //m_smokeFluid.drawGui();

    
    if(AppManager::getInstance().getDebugMode()){
        //m_smokeFluid.drawGui();
    }
    
    ofPopStyle();
    
}

void SmokyHandsScene::willFadeIn() {
    ofLogNotice("SmokyHandsScene::willFadeIn");
    
    m_smokeFluid.reset();
    
    //AppManager::getInstance().getGuiManager().setContourThickness(1.7);
}

void SmokyHandsScene::willDraw() {
    ofLogNotice("SmokyHandsScene::willDraw");
}

void SmokyHandsScene::willFadeOut() {
    ofLogNotice("SmokyHandsScene::willFadeOut");
}

void SmokyHandsScene::willExit() {
    ofLogNotice("SmokyHandsScene::willExit");
    
    m_smokeFluid.reset();
}
