/*
 *  HandsWritingScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 02/07/15.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"

#include "HandsWritingScene.h"


HandsWritingScene::HandsWritingScene():ofxScene("HandsWritingScene"), m_initialized(false), m_skipFrames(0)
{
    
}

HandsWritingScene::~HandsWritingScene()
{
    //Intentionally left empty
}

void HandsWritingScene::setup()
{
    if (m_initialized) {
        return;
    }
    
    m_initialized = true;
    
    this->setupFbo();
    //this->setupFluid();
    this->setupBrush();
    
    
    ofLogNotice("HandsWritingScene::setup");
    
}

void HandsWritingScene::setupFbo()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene*)this);
    m_drawArea = ofRectangle(0, 0, windowsSettings.getWidth(), windowsSettings.getHeight());
    
    m_fbo.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void HandsWritingScene::setupFluid()
{
    m_fluid.setup("xmls/HandsWritingFluid.xml");
}


void HandsWritingScene::setupBrush()
{
    string resourceName = "hands_brush";
    m_brush.setResource(resourceName);
    m_brush.setColor(ofColor::blue);
    m_brush.setCentred(true);
    
    float size = AppManager::getInstance().getHandsManager().getSize();

    m_brush.setWidth(m_brush.getOriginalWidth()*size,true);
}



void HandsWritingScene::update()
{
    //this->updateFluid();
    this->updateHands();
}

void HandsWritingScene::updateHands()
{
    auto hands = AppManager::getInstance().getHandsManager().getHands();
    
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    
    float fadeTime = AppManager::getInstance().getHandsManager().getFadeTime();
    
    float size = AppManager::getInstance().getHandsManager().getSize();
    
    m_brush.setWidth(m_brush.getOriginalWidth()*size,true);
    
    float decrease = 3.0;
    float framesToDie = 255.0/decrease;
    float dt = ofGetLastFrameTime();
    int numSkipFrames = fadeTime/(framesToDie*dt);
    m_skipFrames++;
    
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    m_fbo.begin();
    ofPushStyle();
        if(m_skipFrames>=numSkipFrames){
            ofSetColor(0,0,0,decrease);
            ofDrawRectangle(0,0,m_fbo.getWidth(),m_fbo.getHeight());
            m_skipFrames = 0;
        }
    //ofSetColor(0,0,0,5);
    //ofRect(0,0,m_fbo.getWidth(),m_fbo.getHeight());
    
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (auto hand : hands) {
            
            ofPoint pos = hand;
            pos.x *= windowsSettings.getWidth();
            pos.y *= windowsSettings.getHeight();
            m_brush.setPosition(pos);
            //ofSetColor(255,255,255);
            m_brush.draw();
        }
    ofPopStyle();
    m_fbo.end();
    //ofDisableAlphaBlending();
}


void HandsWritingScene::draw() {
    ofBackground(0);
    this->drawHands();
}

void HandsWritingScene::updateFluid()
{
    auto hands = AppManager::getInstance().getHandsManager().getHands();
    
    //ofLogNotice() <<"HandsManager::readHands << size -> " << hands.size();
    
    for (auto hand : hands) {
        m_fluid.addForce(hand);
         //ofLogNotice() <<"HandsManager::readHands << y -> " << hand.y;
         //ofLogNotice() <<"HandsManager::readHands << x -> " << hand.x;
    }
    
    m_fluid.update();
}

void HandsWritingScene::drawHands()
{

    m_fbo.draw(0, 0);
    
    /* ofEnableBlendMode(OF_BLENDMODE_ADD);
    int maxRadius = 50;  // Increase for a wider brush
    int radiusStepSize = 5;  // Decrease for more circles (i.e. a more opaque brush)
    int alpha = 3;  // Increase for a more opaque brush
    int maxOffsetDistance = 100;  // Increase for a larger spread of circles
    for (int radius=maxRadius; radius>0; radius-=radiusStepSize) {
        
        // Formula for converting from polar to Cartesian coordinates:
        //    x = cos(polar angle) * (polar distance)
        //    y = sin(polar angle) * (polar distance)
        // We need our angle to be in radians if we want to use sin() or cos()
        // so we can make use of an openFrameworks function to convert from degrees
        // to radians
        float angle = ofRandom(ofDegToRad(360.0));
        float distance = ofRandom(maxOffsetDistance);
        float xOffset = cos(angle) * distance;
        float yOffset = sin(angle) * distance;
        
        // Using the ofColor class, we will randomly select a color between orange and red
        ofColor myOrange(255, 132, 0, alpha);
        ofColor myRed(255, 6, 0, alpha);
        ofColor inBetween = myOrange.getLerped(myRed, ofRandom(1.0));
        ofSetColor(inBetween);
        
        ofCircle(ofGetMouseX()+xOffset, ofGetMouseY()+yOffset, radius);
    }*/
    
}

void HandsWritingScene::drawFluid()
{
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    
    //AppManager::getInstance().getHandsManager().draw(m_drawArea);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    m_fluid.draw(m_drawArea);
    
    if(AppManager::getInstance().getDebugMode()){
        m_fluid.drawGui();
    }
    
    ofPopStyle();
    
}


void HandsWritingScene::willFadeIn() {
    ofLogNotice("HandsWritingScene::willFadeIn");
    m_skipFrames = 0;
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    AppManager::getInstance().getGuiManager().setHandsOn(true);
}

void HandsWritingScene::willDraw() {
    ofLogNotice("HandsWritingScene::willDraw");
}

void HandsWritingScene::willFadeOut() {
    ofLogNotice("HandsWritingScene::willFadeOut");
}

void HandsWritingScene::willExit() {
    ofLogNotice("HandsWritingScene::willExit");
    AppManager::getInstance().getGuiManager().setHandsOn(false);
}
