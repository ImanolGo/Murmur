/*
 *  FluidFloorScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 02/07/15.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"
#include "SettingsManager.h"

#include "FluidFloorScene.h"


FluidFloorScene::FluidFloorScene():ofxScene("FluidFloorScene"), m_initialized(false)
{
    
}

FluidFloorScene::~FluidFloorScene()
{
    //Intentionally left empty
}

void FluidFloorScene::setup()
{
    if (m_initialized) {
        return;
    }
    
    m_initialized = true;
    
    m_fluid.setup("xmls/FluidFloor.xml");
    
    this->setupFbos();
    this->setupShaders();
    
    ofLogNotice("FluidFloorScene::setup");
    
}

void FluidFloorScene::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene* ) this);
    m_fboMask.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    m_fboFluid.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    
    ImageVisual gradientMask = ImageVisual(ofPoint(windowsSettings.getWidth()*0.5, windowsSettings.getHeight()*0.5), "floor_mask", true );
    gradientMask.setHeight(windowsSettings.getHeight(),true);
    if(windowsSettings.getHeight()>windowsSettings.getWidth()){
        gradientMask.setHeight(windowsSettings.getWidth(),true);
    }
    
    ofLogNotice()<<"FluidFloorScene::mask width = " << gradientMask.getWidth() << ", height = " << gradientMask.getHeight() ;
    
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

void FluidFloorScene::setupShaders()
{
    if(ofIsGLProgrammableRenderer()){
        m_maskShader.load("shaders/shadersGL3/BlackMask");
    }
    else{
        m_maskShader.load("shaders/shadersGL2/BlackMask");
    }
    
    
}

void FluidFloorScene::update()
{
    this->updateFluid();
}


void FluidFloorScene::draw() {
     ofBackground(0);

    //m_maskShader.begin();
    //m_maskShader.setUniformTexture("imageMask", m_fboMask.getTextureReference(), 1);
        //AppManager::getInstance().getHandsManager().draw();
        //this->drawFluid();
        m_fboFluid.draw(0, 0);
    //m_maskShader.end();
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

void FluidFloorScene::updateFluid()
{
    auto floorPosition = AppManager::getInstance().getFloorManager().getPosition();
    
    m_fluid.addForce(floorPosition);
    m_fluid.update();
    
    m_fboFluid.begin();
        ofBackground(0, 0, 0);
        m_fluid.draw(m_drawArea);
    m_fboFluid.end();
}



void FluidFloorScene::drawFluid()
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


void FluidFloorScene::willFadeIn() {
    ofLogNotice("FluidFloorScene::willFadeIn");
    m_fluid.reset();
}

void FluidFloorScene::willDraw() {
    ofLogNotice("FluidFloorScene::willDraw");
}

void FluidFloorScene::willFadeOut() {
    ofLogNotice("FluidFloorScene::willFadeOut");
}

void FluidFloorScene::willExit() {
    ofLogNotice("FluidFloorScene::willExit");
    
     m_fluid.reset();
}
