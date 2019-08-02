/*
 *  AudioManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 30/07/15.
 *
 */


#include "AudioManager.h"
#include "WindowSettingsManager.h"
#include "AppManager.h"


AudioManager::AudioManager(): m_volume(0.5)
{
    //Intentionaly left empty
}


AudioManager::~AudioManager()
{
    ofLogNotice() <<"AudioManager::Destructor" ;
}


void AudioManager::setup()
{
    Manager::setup();
    
    ofLogNotice() <<"AudioManager::initialized" ;
    
    this->setupText();
    this->setupFFT();
}


void AudioManager::setupFFT()
{
    m_fft.setup();
    m_fft.setMirrorData(false);
    m_fft.setPeakDecay(0.915);
    m_fft.setMaxDecay(0.995);
    m_fft.setThreshold(1.0);
    m_fft.setVolume(m_volume);
}

void AudioManager::setupText()
{
    auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings(0);
    //auto guiManager = AppManager::getInstance().getGuiManager();
    
    ofVec3f position;
    
    float width =  (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5 - LayoutManager::MARGIN;
    int fontSize = 12;
    float height = fontSize*3;
    
    
    string text = "AUDIO";
    float width_offset = (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
    
    position.x = GuiManager::GUI_WIDTH + 2.5*LayoutManager::MARGIN + LayoutManager::MARGIN  + width_offset;
    position.y = LayoutManager::MARGIN + windowSettings.getHeight()*0.5;
    
    ofPtr<TextVisual> textVisual = ofPtr<TextVisual>(new TextVisual(position, width, height));
    textVisual->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    textVisual->setColor(ofColor::white);
    
    AppManager::getInstance().getViewManager().addOverlay(textVisual);
    
    position.x -= LayoutManager::MARGIN*0.5;
    position.y -= LayoutManager::MARGIN*0.5;
    height = textVisual->getHeight() + LayoutManager::MARGIN;
    width = textVisual->getWidth() + LayoutManager::MARGIN;
    ofPtr<RectangleVisual> rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    ofColor color(60,60,60);
    rectangleVisual->setColor(color);
    
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);
    
}

void AudioManager::update()
{
    if (!m_audioOn) {
        return;
    }
    
    m_fft.update();
    m_audioMax = ofMap(m_fft.getAveragePeak(), 0.0, 0.8, 0.0, 1.0, true);
    
    //ofLogNotice() <<"AudioManager::update: " << m_fft.getUnScaledLoudestValue();
    //ofLogNotice() <<"AudioManager::update2: " << m_fft.getLoudBand();
}

void AudioManager::draw()
{
    if (!m_audioOn) {
        return;
    }

    //m_fft.draw(340,600);
    this->drawCircle();
}

void AudioManager::drawCircle()
{
    int mainWindowIndex = 0;
    auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings(mainWindowIndex);
    
    ofPoint pos;
    pos.x = windowSettings.getWidth() -   windowSettings.getWidth()*0.25;
    pos.y = windowSettings.getHeight() -   windowSettings.getHeight()*0.25;
    
    float radio = ofMap(m_fft.getAveragePeak(), 0.0, 1.0, windowSettings.getHeight()*0.05, windowSettings.getHeight()*0.2, true);
    
    
    ofPushStyle();
    ofSetCircleResolution(100);
        ofSetColor(255);
        ofDrawCircle(pos, radio);
    ofPopStyle();
    
  
}


void AudioManager::onChangeVolume(float& value)
{
    m_volume = value;
    m_fft.setVolume(m_volume);
}

void AudioManager::setAudioMax(float audioMax)
{
    if(m_audioOn){
        return;
    }
    
    m_audioMax = audioMax;
}

void AudioManager::onChangeAudioOn(bool& value)
{
    m_audioOn = value;
    
}


float AudioManager::getAudioMax()
{
    return m_audioMax;
}
\
