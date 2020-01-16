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

    this->setupFbo();
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

void SonicBoomScene::setupFbo()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *)this);

    m_drawArea = ofRectangle(0, 0, windowsSettings.getWidth(), windowsSettings.getHeight());
    
    float scale = 0.5;
    m_fbo.allocate(windowsSettings.getWidth()*scale, windowsSettings.getHeight()*scale, GL_RGBA, 4);
    m_fbo.begin(); ofClear(0); m_fbo.end();

}



void SonicBoomScene::update()
{
    this->updateSonicBoom();
    this->updateFbo();
}

void SonicBoomScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0,255);
        this->drawSonicBoom();
    m_fbo.end();
}

void SonicBoomScene::updateSonicBoom()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *)this);
    ofPoint pos = ofPoint(ofRandom(windowsSettings.getWidth()), ofRandom(windowsSettings.getHeight()));
    m_sonicBoomVisual.addParticle(pos);
    m_sonicBoomVisual.update();
}

void SonicBoomScene::draw()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *)this);
    
    //ofLogNotice("SonicBoomScene::draw");
    ofClear(0);
    m_fbo.draw(0,0, windowsSettings.getWidth(), windowsSettings.getHeight());
}


void SonicBoomScene::drawSonicBoom()
{
    if (m_sonicBoomVisual.empty()) {
        return;
    }

    m_shader.begin();
    m_shader.setUniform1f("time", ofGetElapsedTimef());
    m_shader.setUniform1f("frequency", 0.004);
    m_shader.setUniform1f("amplitude", 22.0);
    m_shader.setUniform1f("speed", 0.4);
        m_sonicBoomVisual.draw(m_fbo.getWidth(), m_fbo.getHeight());
    m_shader.end();

  // m_sonicBoomVisual.draw();
    //m_sonicBoomVisual.draw(m_fbo.getWidth(), m_fbo.getHeight());
    
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
