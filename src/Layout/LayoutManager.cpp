/*
 *  LayoutManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "SettingsManager.h"
#include "ViewManager.h"
#include "WindowSettingsManager.h"


#include "LayoutManager.h"

const int LayoutManager::MARGIN = 2;
const int LayoutManager::FRAME_MARGIN = 2;

const string LayoutManager::LAYOUT_FONT =  "fonts/roboto/Roboto-Medium.ttf";
const string LayoutManager::LAYOUT_FONT_LIGHT =  "fonts/roboto/Roboto-Light.ttf";


LayoutManager::LayoutManager(): Manager(), m_cropLeft(0), m_cropRight(0), m_cropTop(0), m_cropBottom(0), m_isMasked(true)
{
    //Intentionally left empty
}


LayoutManager::~LayoutManager()
{
    ofLogNotice() <<"LayoutManager::Destructor";
}


void LayoutManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupFbos();
    this->setupWindowFrames();
    
    this->createTextVisuals();
    this->createImageVisuals();
    
    //this->addVisuals();
    
    ofLogNotice() <<"LayoutManager::initialized";
    
}

void LayoutManager::setupFbos()
{
    int margin = MARGIN;
    
    auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings(1);
  
    ofPtr<ofFbo> fbo = ofPtr<ofFbo>(new ofFbo());
    fbo->allocate(windowSettings.getWidth(), windowSettings.getHeight(), GL_RGBA);
    fbo->begin(); ofClear(0);  fbo->end();
    m_fbos["FRONT"] = fbo;
    
    fbo = ofPtr<ofFbo>(new ofFbo());
    fbo->allocate(windowSettings.getWidth(), windowSettings.getHeight(), GL_RGBA);
    fbo->begin(); ofClear(0);  fbo->end();
    m_fbos["TOP"] = fbo;
}

void LayoutManager::setupWindowFrames()
{
    ofColor color = AppManager::getInstance().getSettingsManager().getColor("FrameRectangle");
    
    for (auto& fbo : m_fbos)
    {
        ofPtr<ofRectangle>  rect = ofPtr<ofRectangle> (new ofRectangle());
        m_windowRects[fbo.first] = rect;
        
        ofPtr<RectangleVisual>  rectVisual = ofPtr<RectangleVisual> (new RectangleVisual());
        m_windowFrames[fbo.first] = rectVisual;
        m_windowFrames[fbo.first]->setColor(color);
    }
    
    this->resetWindowFrames();
    this->resetWindowRects();
}

void LayoutManager::resetWindowRects()
{
    float gui_offset = AppManager::getInstance().getGuiManager().getWidth() +  2*AppManager::getInstance().getGuiManager().getPosition().x;
    float frame_width = ofGetWindowWidth() - gui_offset;
    float frame_height= ofGetWindowHeight();
    
    m_windowRects["FRONT"]->width = frame_width/2 - 2*MARGIN;
    m_windowRects["FRONT"]->height = frame_height - 2*MARGIN;
    m_windowRects["FRONT"]->x = gui_offset  + 2*MARGIN;
    m_windowRects["FRONT"]->y = MARGIN;
    
    m_windowRects["TOP"]->width = m_windowRects["FRONT"]->width;
    m_windowRects["TOP"]->height = m_windowRects["FRONT"]->height;
    m_windowRects["TOP"]->x = gui_offset  + 2*MARGIN;
    m_windowRects["TOP"]->y = MARGIN;
    
    
    for (auto& frame : m_windowFrames)
    {
        float x_offset = frame.second->getPosition().x + FRAME_MARGIN;
        float y_offset = frame.second->getPosition().y + FRAME_MARGIN;
        float max_width = frame.second->getWidth() - 2*FRAME_MARGIN;
        float max_height = frame.second->getHeight() - 2*FRAME_MARGIN;
        
        float ratio = m_fbos[frame.first]->getWidth()/  m_fbos[frame.first]->getHeight();
        
        float height = max_height;
        float width = height*ratio;
        
        if( width > max_width ){
            width = max_width;
            height = width/ratio;
        }
        
        float x = x_offset + max_width*0.5 - width*0.5;
        float y = y_offset + max_height*0.5 - height*0.5;
        
        m_windowRects[frame.first]->width = width;
        m_windowRects[frame.first]->height = height;
        m_windowRects[frame.first]->x = x;
        m_windowRects[frame.first]->y = y;
    }
    
}


void LayoutManager::resetWindowFrames()
{
    float gui_offset = AppManager::getInstance().getGuiManager().getWidth() +  2*AppManager::getInstance().getGuiManager().getPosition().x;
    float frame_width = ofGetWindowWidth() - gui_offset;
    float frame_height= ofGetWindowHeight();
    
    m_windowFrames["FRONT"]->setWidth(frame_width/2 - 2*MARGIN + 2*FRAME_MARGIN);
    m_windowFrames["FRONT"]->setHeight(frame_height - 2*MARGIN + 2*FRAME_MARGIN);
    float x = gui_offset  + 2*MARGIN - FRAME_MARGIN;
    float y =  MARGIN- FRAME_MARGIN;
    m_windowFrames["FRONT"]->setPosition(ofPoint(x,y));
    
    
    m_windowFrames["TOP"]->setWidth( m_windowFrames["FRONT"]->getWidth());
    m_windowFrames["TOP"]->setHeight(m_windowFrames["FRONT"]->getHeight());
    x =  x + m_windowFrames["FRONT"]->getWidth();
    m_windowFrames["TOP"]->setPosition(ofPoint(x,y));
    
}

void LayoutManager::update()
{
    if(!m_initialized)
        return;
    
    this->updateFbos();
}


void LayoutManager::updateFbos()
{
    this->updateFrontFbo();
    this->updateTopFbo();
}

void LayoutManager::updateFrontFbo()
{
    string name = "FRONT";
    this->begin(name);
        ofClear(0, 0, 0, 255);
        ofSetColor(50, 50, 50);
        ofDrawRectangle(0, 0,m_fbos[name]->getWidth(), m_fbos[name]->getHeight());
        ofSetColor(255);
        AppManager::getInstance().getSceneManager().draw(WindowIndex(1));
    this->end(name);
}

void LayoutManager::updateTopFbo()
{
    string name = "TOP";
    this->begin(name);
        ofClear(0, 0, 0, 255);
        ofSetColor(50, 50, 50);
        ofDrawRectangle(0, 0,m_fbos[name]->getWidth(), m_fbos[name]->getHeight());
        ofSetColor(255);
        AppManager::getInstance().getSceneManager().draw(WindowIndex(2));
    this->end(name);
}



void LayoutManager::createTextVisuals()
{
    float size = 14;
    
    for (auto& rect : m_windowRects)
    {
        float w = rect.second->width;
        float h = size;
        float x =  rect.second->x + w*0.5;
        float y =  rect.second->y - h - MARGIN;
        ofPoint pos = ofPoint(x, y);
        string text = rect.first;
        string fontName = LAYOUT_FONT;
        
        auto textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h,true));
        textVisual->setText(text, fontName, size, ofColor::white);
        m_textVisuals[rect.first] = textVisual;
    }
    
}

void LayoutManager::resetWindowTitles()
{
    for (auto& rect : m_windowRects)
    {
        float x =  rect.second->x + rect.second->width*0.5;
        float y =  rect.second->y -  m_textVisuals[rect.first]->getHeight()*0.5 - MARGIN*0.5;
        ofPoint pos = ofPoint(x, y);
        m_textVisuals[rect.first]->setPosition(pos);
    }
}




void LayoutManager::createImageVisuals()
{
    //this->createBackground();
}

void LayoutManager::createBackground()
{
}


void LayoutManager::draw()
{
    this->drawFbos();
}

void LayoutManager::drawFbos()
{
    for (auto fbo : m_fbos)
    {
        m_windowFrames[fbo.first]->draw();
        //fbo.second->draw(m_windowRects[fbo.first]->x, m_windowRects[fbo.first]->y, m_windowRects[fbo.first]->width, m_windowRects[fbo.first]->height);
        //fbo.second->draw(0,0);
        m_fbos[fbo.first]->draw(m_windowRects[fbo.first]->x, m_windowRects[fbo.first]->y, m_windowRects[fbo.first]->width, m_windowRects[fbo.first]->height);
    }
}

void LayoutManager::drawFront()
{
    m_fbos["FRONT"]->draw(0,0);
}

void LayoutManager::drawTop()
{
    m_fbos["TOP"]->draw(0,0);
}

void LayoutManager::begin(string& name)
{
    if(m_fbos.find(name) == m_fbos.end()){
        return;
    }
    
    m_fbos[name]->begin();
    ofClear(0);
    
}

void LayoutManager::end(string& name)
{
    if(m_fbos.find(name) == m_fbos.end()){
        return;
    }
    
    m_fbos[name]->end();
}


void LayoutManager::addVisuals()
{
    int depthLevel = -1;

    for(TextMap::iterator it = m_textVisuals.begin(); it!= m_textVisuals.end(); ++it){
        //AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
    
    for(ImageMap::iterator it = m_imageVisuals.begin(); it!= m_imageVisuals.end(); ++it){
        // AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
}


void LayoutManager::windowResized(int w, int h)
{
    if(!m_initialized){
        return;
    }
    
//    this->resetWindowRects();
//    this->resetWindowFrames();
//    this->resetWindowTitles();
}


void LayoutManager::onCropLeft( int & pixels)
{
    m_cropLeft = pixels;
    AppManager::getInstance().getMaskManager().setMaskWindowFront();
}

void LayoutManager::onCropRight( int & pixels)
{
    m_cropRight = pixels;
    AppManager::getInstance().getMaskManager().setMaskWindowFront();
}

void LayoutManager::onCropTop( int & pixels)
{
    m_cropTop = pixels;
    AppManager::getInstance().getMaskManager().setMaskWindowFront();
}

void LayoutManager::onCropBottom(int & pixels)
{
    m_cropBottom = pixels;
    AppManager::getInstance().getMaskManager().setMaskWindowFront();
}



