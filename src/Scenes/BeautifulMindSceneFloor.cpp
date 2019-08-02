/*
 *  BeautifulMindSceneFloor.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 14/10/15.
 *
 */


#include "AppManager.h"
#include "BeautifulMindSceneFloor.h"




BeautifulMindSceneFloor::BeautifulMindSceneFloor(): ofxScene("BeautifulMindSceneFloor"), m_initialized(false)
{
    //! Intentionally left empty
}


BeautifulMindSceneFloor::~BeautifulMindSceneFloor()
{
    //! Intentionally left empty
}


void BeautifulMindSceneFloor::setup() {
    
    if(m_initialized){
        return;
    }
    
    m_initialized = true;
    
    this->setupVideo();
    
    ofLogNotice("BeautifulMindSceneFloor::setup");
    
}


void BeautifulMindSceneFloor::setupVideo()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    
    string videoFileName = "videos/alpha rays.mov";
    m_video.setResource(videoFileName);
    m_video.setHeight(windowsSettings.getHeight(),true);
    m_video.setCentred(true);
    
    float x = windowsSettings.getWidth()*0.5;
    float y = windowsSettings.getHeight()*0.5;
    m_video.setPosition(ofPoint(x,y));
    m_video.setLoopState(OF_LOOP_NONE);
}



void BeautifulMindSceneFloor::update()
{
    this->updateVideo();
}

void BeautifulMindSceneFloor::updateVideo()
{
    bool isFloorOn = AppManager::getInstance().getBeautifulMindManager().isFloorOn();
    if(isFloorOn)
    {
        float speed = AppManager::getInstance().getBeautifulMindManager().getFloorSpeed();
        m_video.setSpeed(speed);
        m_video.update();
    }
    
}


void BeautifulMindSceneFloor::draw() {
   
    ofBackground(0);
    
    bool isFloorOn = AppManager::getInstance().getBeautifulMindManager().isFloorOn();
    if(isFloorOn)
    {
        m_video.draw();
    }
}

void BeautifulMindSceneFloor::willFadeIn() {
     ofLogNotice("BeautifulMindSceneFloor::willFadeIn");
    
     m_video.stop();
     m_video.setFrame(0);
     m_video.play();
    
}

void BeautifulMindSceneFloor::willDraw() {
    ofLogNotice("BeautifulMindSceneFloor::willDraw");
}

void BeautifulMindSceneFloor::willFadeOut() {
    ofLogNotice("BeautifulMindSceneFloor::willFadeOut");
}

void BeautifulMindSceneFloor::willExit() {
    ofLogNotice("BeautifulMindSceneFloor::willExit");
    
    m_video.stop();
}
