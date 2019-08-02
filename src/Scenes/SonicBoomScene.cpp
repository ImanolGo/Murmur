/*
 *  SonicBoomScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 21/03/16.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"

#include "SonicBoomScene.h"


SonicBoomScene::SonicBoomScene():ofxScene("SonicBoomScene"), m_initialized(false)
{

}

SonicBoomScene::~SonicBoomScene()
{
    //Intentionally left empty
}

void SonicBoomScene::setup()
{
    if (m_initialized) {
        return;
    }

    this->setupFbos();
    this->setupShaders();

    m_sonicBoomVisual.setup();

    m_initialized = true;

    ofLogNotice("SonicBoomScene::setup");

}

void SonicBoomScene::setupShaders()
{
    
    if(ofIsGLProgrammableRenderer()){
        m_shader.load("shaders/shadersGL3/LiquifyShader");
    }
    else{
        m_shader.load("shaders/shadersGL2/LiquifyShader");
        
    }
}

void SonicBoomScene::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *)this);

    m_drawArea = ofRectangle(0, 0, windowsSettings.getWidth(), windowsSettings.getHeight());

    m_fbo.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    m_fbo.begin(); ofClear(0); m_fbo.end();

}



void SonicBoomScene::update()
{
    this->updateSonicBoom();
}



void SonicBoomScene::updateSonicBoom()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *)this);
    ofPoint pos = ofPoint(ofRandom(windowsSettings.getWidth()), ofRandom(windowsSettings.getHeight()));
    m_sonicBoomVisual.addParticle(pos);
    m_sonicBoomVisual.update();
}

void SonicBoomScene::draw() {

    //ofLogNotice("SonicBoomScene::draw");

    ofBackground(0);
    ofPushStyle();
        //ofEnableBlendMode(OF_BLENDMODE_DISABLED);

        this->drawSonicBoom();

       // ofEnableBlendMode(OF_BLENDMODE_ADD);

    ofPopStyle();

}


void SonicBoomScene::drawSonicBoom()
{
    if (m_sonicBoomVisual.empty()) {
        return;
    }
    
    m_shader.begin();
    m_shader.setUniform1f("time", ofGetElapsedTimef());
    m_shader.setUniform1f("frequency", 0.5);
    m_shader.setUniform1f("amplitude", 22.0);
    m_shader.setUniform1f("speed", 0.5);
        m_sonicBoomVisual.draw();
    m_shader.end();
    
}




void SonicBoomScene::willFadeIn() {
    ofLogNotice("SonicBoomScene::willFadeIn");
    
    AppManager::getInstance().getGuiManager().setHandsOn(false);
    m_sonicBoomVisual.clear();

}

void SonicBoomScene::willDraw() {
    ofLogNotice("SonicBoomScene::willDraw");
}

void SonicBoomScene::willFadeOut() {
    ofLogNotice("SonicBoomScene::willFadeOut");
}

void SonicBoomScene::willExit() {
    ofLogNotice("SonicBoomScene::willExit");

    m_sonicBoomVisual.clear();
}
