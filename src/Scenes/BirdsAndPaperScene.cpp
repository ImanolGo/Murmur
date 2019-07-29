/*
 *  BirdsAndPaperScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 22/09/15.
 *
 */

#include "AppManager.h"

#include "BirdsAndPaperScene.h"


BirdsAndPaperScene::BirdsAndPaperScene(): ofxScene("BirdsAndPaperScene"), m_initialized(false)
{
    //! Intentionally left empty
}


BirdsAndPaperScene::~BirdsAndPaperScene()
{
    //! Intentionally left empty
}


void BirdsAndPaperScene::setup() {
    ofLogNotice("BirdsAndPaperScene::setup");
    this->setInitialParameters();
}


void BirdsAndPaperScene::update() {
    
    //AppManager::getInstance().getBirdsManager().update();
    
}

void BirdsAndPaperScene::draw() {
    ofBackground(0,0,0);
    AppManager::getInstance().getBirdsManager().draw();
}

void BirdsAndPaperScene::willFadeIn() {
    ofLogNotice("BirdsAndPaperScene::willFadeIn");
    
    this->setInitialParameters();
    AppManager::getInstance().getGuiManager().update();
}


void BirdsAndPaperScene::setInitialParameters()
{
    AppManager::getInstance().getBirdsManager().stopEffects();
    
    m_visual.setScale(ofVec3f(3.0,3.0,3.0));
    m_visual.setPosition(ofVec3f(0,0,0));
    
    //AppManager::getInstance().getGuiManager().setBirdsSize(1.2);
    //AppManager::getInstance().getGuiManager().setBirdsNumber(0);
    
    AppManager::getInstance().getGuiManager().setBirdsNumber(100);
    
    AppManager::getInstance().getGuiManager().setBirdsPosition(m_visual.getPosition());
    //AppManager::getInstance().getGuiManager().setBirdsSize(m_visual.getScale().x);
    
    AppManager::getInstance().getGuiManager().setBirdsSpeed(7.0);
    AppManager::getInstance().getGuiManager().setBirdsSize(m_visual.getScale());
    
     AppManager::getInstance().getGuiManager().setBirdsRandomness(0);
}

void BirdsAndPaperScene::willDraw()
{
    ofLogNotice("BirdsAndPaperScene::willDraw");
    this->startBirds();
}

void BirdsAndPaperScene::startBirds()
{
    m_visual.setScale(ofVec3f(20,20,20));
    m_visual.setPosition(ofVec3f(0,0,0));
    m_visual.m_speed = 15;
    m_visual.m_randomness = 30;
    m_visual.m_number = 600;
    
    double delayTime = 20;
    double animationTime = 60;
    
    AppManager::getInstance().getBirdsManager().addMoveEffect(m_visual.getPosition(), animationTime, delayTime);
    AppManager::getInstance().getBirdsManager().addScaleEffect(m_visual.getScale(), animationTime, delayTime);
    AppManager::getInstance().getBirdsManager().addSpeedEffect(m_visual.m_speed, animationTime, delayTime);
    AppManager::getInstance().getBirdsManager().addNumberEffect(m_visual.m_number, animationTime, delayTime);
    AppManager::getInstance().getBirdsManager().addRandomnessEffect(m_visual.m_randomness, animationTime, delayTime);
    
    //AppManager::getInstance().getGuiManager().setBirdsPosition(m_visual.getPosition());
    //AppManager::getInstance().getGuiManager().setBirdsSize(m_visual.getScale());
}

void BirdsAndPaperScene::willFadeOut() {
    ofLogNotice("BirdsAndPaperScene::willFadeOut");
    AppManager::getInstance().getBirdsManager().stopEffects();
}

void BirdsAndPaperScene::willExit() {
    ofLogNotice("BirdsAndPaperScene::willExit");
    this->setInitialParameters();
    AppManager::getInstance().getGuiManager().setBirdsNumber(0);
    
}
