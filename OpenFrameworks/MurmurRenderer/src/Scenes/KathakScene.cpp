/*
 *  KathakScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 30/07/15.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"
#include "SettingsManager.h"

#include "KathakScene.h"


KathakScene::KathakScene(): ofxScene("KathakScene"), m_initialized(false)
{
    
}

KathakScene::~KathakScene()
{
    //Intentionally left empty
}

void KathakScene::setup()
{
    if (m_initialized) {
        return;
    }
    
    m_initialized = true;
    
    this->setupFbos();
    this->setupShaders();
    this->setupWaterRipples();
    
    ofLogNotice("KathakScene::setup");
    
}

void KathakScene::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings( (ofxScene *)this);
    m_fboMask.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    
    ImageVisual gradientMask = ImageVisual(ofPoint(windowsSettings.getWidth()*0.5, windowsSettings.getHeight()*0.5), "floor_mask", true );
    gradientMask.setHeight(windowsSettings.getHeight(), true);
    if(windowsSettings.getHeight()>windowsSettings.getWidth()){
        gradientMask.setHeight(windowsSettings.getWidth(),true);
    }
    
    m_fboMask.begin();
        ofClear(0);
        gradientMask.draw();
    m_fboMask.end();
    
    m_drawArea = ofRectangle(0, 0, windowsSettings.getWidth(), windowsSettings.getHeight());
}

void KathakScene::setupShaders()
{
    if(ofIsGLProgrammableRenderer()){
        m_maskShader.load("shaders/shadersGL3/BlackMask");
    }
    else{
        m_maskShader.load("shaders/shadersGL2/BlackMask");
    }
    
}

void KathakScene::setupWaterRipples()
{
    ofSetCircleResolution(100);
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *)this);
    
    ofImage waterBackground;
    waterBackground.allocate(windowsSettings.getWidth(), windowsSettings.getHeight(), OF_IMAGE_GRAYSCALE);
    m_water.loadBackground(waterBackground);
    m_water.setDensity(0.97);
    
}

void KathakScene::update()
{
    this->updateWaterRipples();
}


void KathakScene::draw() {
     ofClear(0);
    
    //m_maskShader.begin();
    //m_maskShader.setUniformTexture("imageMask", m_fboMask.getTextureReference(), 1);

        this->drawWaterRipples();
    //m_maskShader.end();
    

}

void KathakScene::updateWaterRipples()
{
    float volume = AppManager::getInstance().getAudioManager().getAudioMax();
    auto position = AppManager::getInstance().getFloorManager().getPosition();
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings( (ofxScene *) this);
    
    position.x *= windowsSettings.getWidth();
    position.y *= windowsSettings.getHeight();
    
    //ofLogNotice() << "KathakScene::updateWaterDrops: " << volume;
    
    m_water.begin();
        ofNoFill();
        ofSetLineWidth(20);
        ofColor color = ofColor::lightSeaGreen;
        color.setSaturation( 50 + ofNoise( ofGetFrameNum() ) * 255 );
        ofSetColor(color);
        //ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
        //ofSetColor(ofColor::blue);
        float radius = ofMap(volume, 0.0, 1.0, 70, windowsSettings.getHeight()/2.5);
        ofDrawCircle(position, radius);
        //ofCircle(ofGetMouseX(), ofGetMouseY(), radius);
    m_water.end();
    
    m_water.update();

}


void KathakScene::drawWaterRipples()
{
    m_water[1].draw(0,0);
}


void KathakScene::willFadeIn() {
    ofLogNotice("KathakScene::willFadeIn");
}

void KathakScene::willDraw() {
    ofLogNotice("KathakScene::willDraw");
}

void KathakScene::willFadeOut() {
    ofLogNotice("KathakScene::willFadeOut");
}

void KathakScene::willExit() {
    ofLogNotice("KathakScene::willExit");
}
