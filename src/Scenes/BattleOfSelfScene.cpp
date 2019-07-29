/*
 *  BattleOfSelfScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 30/06/15.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"

#include "BattleOfSelfScene.h"


BattleOfSelfScene::BattleOfSelfScene(): ofxScene("BattleOfSelfScene"), m_initialized(false)
{
    
}

BattleOfSelfScene::~BattleOfSelfScene()
{
    //Intentionally left empty
}

void BattleOfSelfScene::setup()
{
    if (m_initialized) {
        return;
    }
    
    this->setupFbos();
    this->setupShaders();
    
    m_sonicBoomVisual.setup();
    
    m_fluid.setup("xmls/BattleOfSelfFluid.xml");
    m_initialized = true;
    
    ofLogNotice("BattleOfSelfScene::setup");
    
}

void BattleOfSelfScene::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *) this);
    
    m_drawArea = ofRectangle(0, 0, windowsSettings.getWidth(), windowsSettings.getHeight());
    
    m_fbo.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
}



void BattleOfSelfScene::setupShaders()
{
    
    if(ofIsGLProgrammableRenderer()){
        m_shader.load("shaders/shadersGL3/LiquifyShader");
    }
    else{
        m_shader.load("shaders/shadersGL2/LiquifyShader");
        
    }
}

void BattleOfSelfScene::update()
{
    this->updateFluid();
    this->updateSonicBoom();
}


void BattleOfSelfScene::updateFluid()
{
    auto source = AppManager::getInstance().getContourManager().getSource();
    
    //source = m_sonicBoomVisual.getFbo();
    m_fluid.setSource(source);
    m_fluid.update();
}


void BattleOfSelfScene::updateSonicBoom()
{
    m_sonicBoomVisual.update();
}

void BattleOfSelfScene::draw() {
    
    //ofLogNotice("BattleOfSelfScene::draw");

    ofClear(0, 0, 0);
    ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    
        this->drawSonicBoom();
    
        ofEnableBlendMode(OF_BLENDMODE_ADD);

        this->drawFluid();
    
    ofPopStyle();
    
}


void BattleOfSelfScene::drawSonicBoom()
{
    if (m_sonicBoomVisual.empty()) {
        return;
    }
    
    
    m_shader.begin();
    m_shader.setUniform1f("time", ofGetElapsedTimef());
    m_shader.setUniform1f("frequency", 5);
    m_shader.setUniform1f("amplitude", 0.01);
    m_shader.setUniform1f("speed", 0.3);
        m_sonicBoomVisual.draw();
    m_shader.end();

}

void BattleOfSelfScene::drawFluid()
{
    
    //ofSetColor(160,160,255);
    //m_fluid.draw(m_drawArea);
    //return;
    
    ofPushStyle();
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofEnableAlphaBlending();
    
    m_fbo.begin();
    
    //ofClear(0, 0, 0);
    
    ofSetColor(0,0,0,80);
    ofDrawRectangle(0,0,m_fbo.getWidth(),m_fbo.getHeight());
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(160,160,255);
    m_fluid.draw(m_drawArea);
    
    m_fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    m_fbo.draw(0,0);
    
    //m_fluid.drawGui();
    
}


void BattleOfSelfScene::willFadeIn() {
    ofLogNotice("BattleOfSelfScene::willFadeIn");
    
    m_sonicBoomVisual.clear();
    m_fluid.reset();
    AppManager::getInstance().getGuiManager().setHandsOn(false);
    
    //AppManager::getInstance().getGuiManager().setContourThickness(0.25);
}

void BattleOfSelfScene::willDraw() {
    ofLogNotice("BattleOfSelfScene::willDraw");
}

void BattleOfSelfScene::willFadeOut() {
    ofLogNotice("BattleOfSelfScene::willFadeOut");
}

void BattleOfSelfScene::willExit() {
    ofLogNotice("BattleOfSelfScene::willExit");
    
    m_sonicBoomVisual.clear();
    m_fluid.reset();
}
