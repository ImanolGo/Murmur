/*
 *  PostShowScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 21/10/15.
 *
 */


#include "PostShowScene.h"
#include "AppManager.h"

PostShowScene::PostShowScene(): ofxScene("PostShowScene"), m_initialized(false)
{
    
}

PostShowScene::~PostShowScene()
{
    //Intentionally left empty
}

void PostShowScene::setup() {
    ofLogNotice("PostShowScene::setup");
    
    this->setupImages();
}

void PostShowScene::setupImages() {
   
    string imageName = "postshow";
    m_image.setResource(imageName);
    m_image.setCentred(true);

}

void PostShowScene::update() {
    
}

void PostShowScene::draw() {
    ofBackground(0,0,0);
    this->drawImages();
}

void PostShowScene::drawImages() {
    ofRectangle rect = this->getDrawingArea();
    //m_image.setHeight(rect.getHeight());
    m_image.setHeight(400,true);
    m_image.setPosition(rect.getCenter());
    m_image.draw();
}

void PostShowScene::willFadeIn() {
     ofLogNotice("PostShowScene::willFadeIn");
}

void PostShowScene::willDraw() {
    ofLogNotice("PostShowScene::willDraw");
}

void PostShowScene::willFadeOut() {
    ofLogNotice("PostShowScene::willFadeOut");
}

void PostShowScene::willExit() {
    ofLogNotice("PostShowScene::willExit");
}

ofRectangle PostShowScene::getDrawingArea()
{
    auto rect = AppManager::getInstance().getBeautifulMindManager().getRectangleSpace();
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    
    rect.x *= windowsSettings.getWidth();
    rect.width *= windowsSettings.getWidth();
    rect.y *= windowsSettings.getHeight();
    rect.height *= windowsSettings.getHeight();
    
    return rect;
}
