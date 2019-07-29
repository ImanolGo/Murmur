/*
 *  PreviewManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 29/09/15.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "PreviewManager.h"
#include "WindowSettingsManager.h"



PreviewManager::PreviewManager(): Manager()
{
	//Intentionally left empty
}


PreviewManager::~PreviewManager()
{
    ofLogNotice() <<"PreviewManager::Destructor";
}


void PreviewManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();
    
    this->setupFbos();
    this->setupText();
    this->setupRectangles();
    
    ofLogNotice() <<"PreviewManager::initialized";

}

void PreviewManager::setupFbos()
{
    auto windowSettingsVector = WindowSettingsManager::getInstance().getWindowsSettings();
    
    for(auto windowSettings : windowSettingsVector)
    {
        ofPtr<ofFbo> fbo =  ofPtr<ofFbo>(new ofFbo());
        fbo->allocate(windowSettings.getWidth(), windowSettings.getHeight(), GL_RGBA);
        ofLogNotice() <<"PreviewManager::setupFbos -> width =  " << windowSettings.getWidth() << ", height =  " << windowSettings.getHeight();

        fbo->begin(); ofClear(0); fbo->end();
        m_fbos.push_back(fbo);
    }

    
}

void PreviewManager::setupText()
{
    ofVec3f position;
    
    position.x = GuiManager::GUI_WIDTH + 2.5*LayoutManager::MARGIN;
    position.y = 1.5*LayoutManager::MARGIN;
    
    int width = 700;
    int fontSize = 12;
    int height = fontSize*3;
  
    string text = "FRONT";
    ofPtr<TextVisual> textVisual = ofPtr<TextVisual>(new TextVisual(position, width, height));
    textVisual->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    textVisual->setColor(ofColor::white);
    
    m_texts[text] = textVisual;
    AppManager::getInstance().getViewManager().addOverlay(textVisual);
    
    int indexWindow = 1;
    int width_offset = 0;
    auto windowSettings =  WindowSettingsManager::getInstance().getWindowsSettings(0);
    
    if(indexWindow > 0 ||  indexWindow < m_fbos.size()){
        width_offset = (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
    }
    
    
    position.x = GuiManager::GUI_WIDTH + 2.5*LayoutManager::MARGIN + LayoutManager::MARGIN  + width_offset;
    position.y = 1.5*LayoutManager::MARGIN;
    
    text = "TOP";
    textVisual = ofPtr<TextVisual>(new TextVisual(position, width, height));
    textVisual->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    textVisual->setColor(ofColor::white);
    m_texts[text] = textVisual;
    
    AppManager::getInstance().getViewManager().addOverlay(textVisual);
    
}

void PreviewManager::setupRectangles()
{
    string key = "FRONT";
    ofVec3f position = m_texts[key]->getPosition();
    position.x -= LayoutManager::MARGIN*0.5;
    position.y -= LayoutManager::MARGIN*0.5;
    float height = m_texts[key]->getHeight() + LayoutManager::MARGIN;
    float width = m_texts[key]->getWidth() + LayoutManager::MARGIN;
    ofPtr<RectangleVisual> rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    ofColor color(60,60,60);
    rectangleVisual->setColor(color);
    
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);
    
    
    key = "TOP";
    position = m_texts[key]->getPosition();
    position.x -= LayoutManager::MARGIN*0.5;
    position.y -= LayoutManager::MARGIN*0.5;
    height = m_texts[key]->getHeight() + LayoutManager::MARGIN;
    width = m_texts[key]->getWidth() + LayoutManager::MARGIN;
    rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    rectangleVisual->setColor(color);
    
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);
    
    
    key = "FRONT";
    int indexWindow = 1;
    if(indexWindow < m_fbos.size()){
        auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings(0);
        
        position.y = 3*LayoutManager::MARGIN;
        position.x = 2*LayoutManager::MARGIN + GuiManager::GUI_WIDTH;
        
        if(m_fbos[indexWindow]->getWidth()> m_fbos[indexWindow]->getHeight()){
            width = (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
            height = width *  m_fbos[indexWindow]->getHeight()/  m_fbos[indexWindow]->getWidth();
        }
        else{
            height = (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
            width = height *  m_fbos[indexWindow]->getWidth()/  m_fbos[indexWindow]->getHeight();
        }
        //height = windowSettings.height * 0.4;
        //width = height * m_fbos[indexWindow]->getWidth() / m_fbos[indexWindow]->getHeight();
        
    }
    
    rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    rectangleVisual->setColor(color);
    
    m_rectangles[key] = rectangleVisual;
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);
    
    
    key = "TOP";
    indexWindow = 2;
    if(indexWindow < m_fbos.size()){
        auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings(0);
        
        position.x = 3*LayoutManager::MARGIN + GuiManager::GUI_WIDTH + width;
        position.y = 3*LayoutManager::MARGIN;
        
    
        if(m_fbos[indexWindow]->getWidth()> m_fbos[indexWindow]->getHeight()){
            width = (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
            height = width *  m_fbos[indexWindow]->getHeight()/  m_fbos[indexWindow]->getWidth();
        }
        else{
            height = (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
            width = height *  m_fbos[indexWindow]->getWidth()/  m_fbos[indexWindow]->getHeight();
        }
       
        
    }
    
    rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    rectangleVisual->setColor(color);
    
    m_rectangles[key] = rectangleVisual;
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);


}

void PreviewManager::draw()
{
    this->drawPreviewWindowFront();
    this->drawPreviewWindowTop();
}

void PreviewManager::drawPreviewWindowFront()
{
    int indexWindow = 1;
    
    if(indexWindow<0 ||  indexWindow > (m_fbos.size()-1)){
        return;
    }

    string key = "FRONT";
    
    float w = m_rectangles[key]->getWidth() - LayoutManager::MARGIN;
    float h = m_rectangles[key]->getHeight() - LayoutManager::MARGIN;

    
    ofVec3f position = m_rectangles[key]->getPosition();
    position.y += 0.5*LayoutManager::MARGIN;
    position.x += 0.5*LayoutManager::MARGIN;
    
    
    m_fbos[indexWindow]->draw(position.x, position.y, w, h);
}

void PreviewManager::drawPreviewWindowTop()
{
    int indexWindow = 2;
    
    if(indexWindow<0 ||  indexWindow > (m_fbos.size()-1)){
        return;
    }
    
    
    string key = "TOP";
    
    float w = m_rectangles[key]->getWidth() - LayoutManager::MARGIN;
    float h = m_rectangles[key]->getHeight() - LayoutManager::MARGIN;
    
    
    ofVec3f position = m_rectangles[key]->getPosition();
    position.y += 0.5*LayoutManager::MARGIN;
    position.x += 0.5*LayoutManager::MARGIN;
    
    m_fbos[indexWindow]->draw(position.x, position.y, w, h);
}


void PreviewManager::draw(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_fbos.size()-1)){
        return;
    }
    
    m_fbos[windowIndex]->draw(0,0);
}


void PreviewManager::begin(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_fbos.size()-1)){
        return;
    }
    
    m_fbos[windowIndex]->begin();
    ofClear(0, 0, 0, 0);
}

void PreviewManager::end(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_fbos.size()-1)){
        return;
    }
    
    m_fbos[windowIndex]->end();

}




