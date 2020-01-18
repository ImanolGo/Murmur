/*
 *  BirdsOpeningScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 22/09/15.
 *
 */

#include "AppManager.h"

#include "BirdsOpeningScene.h"

BirdsOpeningScene::BirdsOpeningScene(): ofxScene("BirdsOpeningScene"), m_initialized(false)
{
    
}

BirdsOpeningScene::~BirdsOpeningScene()
{
    //Intentionally left empty
}


void BirdsOpeningScene::setup() {
    ofLogNotice("BirdsOpeningScene::setup");
    
    this->setInitialParameters();
}


void BirdsOpeningScene::update() {
    
    //AppManager::getInstance().getBirdsManager().update();
}

void BirdsOpeningScene::draw() {
    ofClear(0);
    AppManager::getInstance().getBirdsManager().draw();
}

void BirdsOpeningScene::willFadeIn() {
    ofLogNotice("BirdsOpeningScene::willFadeIn");
    
    this->setInitialParameters();
    AppManager::getInstance().getGuiManager().update();
}


void BirdsOpeningScene::setInitialParameters() {
    
    AppManager::getInstance().getBirdsManager().stopEffects();
    //AppManager::getInstance().getGuiManager().setBirdsNumber(0);

    //AppManager::getInstance().getGuiManager().setBirdsSize(2.0);
    //AppManager::getInstance().getGuiManager().setBirdsNumber(100);
    
    ofVec3f position(0,-0.6,0);
    AppManager::getInstance().getGuiManager().setBirdsPosition(position);
    
    m_swarmSize = 4.0;
    AppManager::getInstance().getGuiManager().onSetBirdsSwarmSize(m_swarmSize);
    
    AppManager::getInstance().getGuiManager().setBirdsSpeed(7);
    
}

void BirdsOpeningScene::willDraw() {
    ofLogNotice("BirdsOpeningScene::willDraw");
    
    this->startBirds();
}

void BirdsOpeningScene::startBirds()
{
    //AppManager::getInstance().getGuiManager().setBirdsSize(2.0);
    //AppManager::getInstance().getGuiManager().setBirdsNumber(0);
    AppManager::getInstance().getGuiManager().setBirdsNumber(100);
    
    ofVec3f position(0, -0.6, 0);
    AppManager::getInstance().getGuiManager().setBirdsPosition(position);
    
    position = ofVec3f(0,0.0,0);
    AppManager::getInstance().getBirdsManager().addMoveEffect(position, 1,2);
    
    m_swarmSize = 4.0;
    AppManager::getInstance().getGuiManager().onSetBirdsSwarmSize(m_swarmSize);
    
    AppManager::getInstance().getGuiManager().setBirdsRandomness(25);
    

}

void BirdsOpeningScene::willFadeOut() {
    ofLogNotice("BirdsOpeningScene::willFadeOut");
    //AppManager::getInstance().getGuiManager().setBirdsNumber(0);
}

void BirdsOpeningScene::willExit() {
    ofLogNotice("BirdsOpeningScene::willExit");
    //this->setInitialParameters();
    AppManager::getInstance().getBirdsManager().stopEffects();
    AppManager::getInstance().getGuiManager().setBirdsNumber(0);
}
