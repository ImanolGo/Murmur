/*
 *  BirdsAndPaperSceneFloor.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 15/10/15.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"
#include "SettingsManager.h"

#include "BirdsAndPaperSceneFloor.h"


BirdsAndPaperSceneFloor::BirdsAndPaperSceneFloor(): ofxScene("BirdsAndPaperSceneFloor"), m_initialized(false)
{
    
}

BirdsAndPaperSceneFloor::~BirdsAndPaperSceneFloor()
{
    //Intentionally left empty
}

void BirdsAndPaperSceneFloor::setup()
{
    if (m_initialized) {
        return;
    }
    
    m_initialized = true;
    
    m_fluid.setup("xmls/FluidFloor.xml");
    
    this->setupFbos();
    this->setupShaders();
    
    ofLogNotice("BirdsAndPaperSceneFloor::setup");
    
}

void BirdsAndPaperSceneFloor::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *)this);
    m_fboMask.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    m_fboFluid.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    
    ImageVisual gradientMask = ImageVisual(ofPoint(windowsSettings.getWidth()*0.5, windowsSettings.getHeight()*0.5), "floor_mask", true );
    gradientMask.setHeight(windowsSettings.getHeight(),true);
    if(windowsSettings.getHeight()>windowsSettings.getWidth()){
        gradientMask.setHeight(windowsSettings.getWidth(),true);
    }
    
    ofLogNotice()<<"BirdsAndPaperSceneFloor::mask width = " << gradientMask.getWidth() << ", height = " << gradientMask.getHeight() ;
    
    m_fboMask.begin();
        ofClear(0);
        ofBackground(0, 0, 0);
        gradientMask.draw();
    m_fboMask.end();
    
    m_fboFluid.begin();
        ofClear(0);
    m_fboFluid.end();
    
    m_drawArea = ofRectangle(0,0, windowsSettings.getWidth(), windowsSettings.getHeight());
}

void BirdsAndPaperSceneFloor::setupShaders()
{
    if(ofIsGLProgrammableRenderer()){
        m_maskShader.load("shaders/shadersGL3/BlackMask");
    }
    else{
        m_maskShader.load("shaders/shadersGL2/BlackMask");
    }
    
    
}

void BirdsAndPaperSceneFloor::update()
{
    this->updateFluid();
}


void BirdsAndPaperSceneFloor::draw() {
    ofClear(0);

    m_maskShader.begin();
    m_maskShader.setUniformTexture("imageMask", m_fboMask.getTextureReference(), 1);
        //AppManager::getInstance().getHandsManager().draw();
        //this->drawFluid();
        m_fboFluid.draw(0, 0);
    m_maskShader.end();
    //m_maskShader.end();
    //ofPushStyle();
    //ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //m_fboMask.draw(0,0);
    //ofPopStyle();
    
    
    //ofEnableAlphaBlending();
    //this->drawFluid();
    
     //ofEnableAlphaBlending();
    

}

void BirdsAndPaperSceneFloor::updateFluid()
{
    auto floorPosition = AppManager::getInstance().getFloorManager().getPosition();
    
    m_fluid.addForce(floorPosition);
    m_fluid.update();
    
    m_fboFluid.begin();
        ofBackground(0, 0, 0);
        m_fluid.draw(m_drawArea);
    m_fboFluid.end();
}



void BirdsAndPaperSceneFloor::drawFluid()
{
    ofPushMatrix();
    ofPushStyle();
    //ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    
        //AppManager::getInstance().getFloorManager().draw(m_drawArea);
    
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
        m_fluid.draw(m_drawArea);
    
    ofPopStyle();
    ofPopMatrix();
    
}


void BirdsAndPaperSceneFloor::willFadeIn() {
    ofLogNotice("BirdsAndPaperSceneFloor::willFadeIn");
    m_fluid.reset();
}

void BirdsAndPaperSceneFloor::willDraw() {
    ofLogNotice("BirdsAndPaperSceneFloor::willDraw");
}

void BirdsAndPaperSceneFloor::willFadeOut() {
    ofLogNotice("BirdsAndPaperSceneFloor::willFadeOut");
}

void BirdsAndPaperSceneFloor::willExit() {
    ofLogNotice("BirdsAndPaperSceneFloor::willExit");
    
     m_fluid.reset();
}
