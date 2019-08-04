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

const int LayoutManager::MARGIN = 10;
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
    
    this->resetWindowRects();
    this->resetWindowFrames();
    
}

void LayoutManager::resetWindowRects()
{
    float gui_offset = AppManager::getInstance().getGuiManager().getWidth() +  AppManager::getInstance().getGuiManager().getPosition().x;
    float frame_width = ofGetWindowWidth() - gui_offset;
    float frame_height= ofGetWindowHeight()*0.5;
    
    
    float max_width = frame_width - 2*MARGIN;
    float max_height = frame_height - 2*MARGIN;
    float x_offset = gui_offset  + MARGIN;
    float y_offset = MARGIN;
    string key = "FRONT";
    
    float ratio = m_fbos[key]->getWidth()/  m_fbos[key]->getHeight();
    float height = max_height;
    float width = height*ratio;
    if( width > max_width ){
        width = max_width;
        height = width/ratio;
    }
    
    float x = x_offset + max_width*0.5 - width*0.5;
    float y = y_offset + max_height*0.5 - height*0.5;
    
    m_windowRects[key]->width = width;
    m_windowRects[key]->height = height;
    m_windowRects[key]->x = x;
    m_windowRects[key]->y = y;
    
    
    y_offset +=  m_windowFrames[key]->getHeight() + MARGIN;
    key = "TOP";
    
    ratio = m_fbos[key]->getWidth()/  m_fbos[key]->getHeight();
    height = max_height;
    width = height*ratio;
    if( width > max_width ){
        width = max_width;
        height = width/ratio;
    }
    
    x = x_offset + max_width*0.5 - width*0.5;
    y = y_offset + max_height*0.5 - height*0.5;
    
    m_windowRects[key]->width = width;
    m_windowRects[key]->height = height;
    m_windowRects[key]->x = x;
    m_windowRects[key]->y = y;
    
}


void LayoutManager::resetWindowFrames()
{
    for (auto& rect : m_windowRects)
    {
        m_windowFrames[rect.first]->setPosition(ofPoint(rect.second->x - FRAME_MARGIN, rect.second->y - FRAME_MARGIN));
        m_windowFrames[rect.first]->setWidth(rect.second->width + 2*FRAME_MARGIN);
        m_windowFrames[rect.first]->setHeight(rect.second->height+ 2*FRAME_MARGIN);
    }
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
    int index = 1;
    string name = "FRONT";
    this->begin(name);
    if(m_isMasked){
        AppManager::getInstance().getMaskManager().begin(WindowIndex(index));
    }
        ofClear(0,255);
        //ofSetColor(255);
        AppManager::getInstance().getSceneManager().draw(WindowIndex(index));
    if(m_isMasked){
        AppManager::getInstance().getMaskManager().end(WindowIndex(index));
    }
    this->end(name);
}

void LayoutManager::updateTopFbo()
{
    int index = 2;
    string name = "TOP";
    this->begin(name);
    if(m_isMasked){
        AppManager::getInstance().getMaskManager().begin(WindowIndex(index));
    }
        ofClear(0,255);
        //ofSetColor(255);
        AppManager::getInstance().getSceneManager().draw(WindowIndex(index));
    if(m_isMasked){
        AppManager::getInstance().getMaskManager().end(WindowIndex(index));
    }
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
        ofSetColor(0);
        ofDrawRectangle(m_windowRects[fbo.first]->x, m_windowRects[fbo.first]->y, m_windowRects[fbo.first]->width, m_windowRects[fbo.first]->height);
        ofSetColor(255);
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
    
    this->resetWindowRects();
    this->resetWindowFrames();
    this->resetWindowTitles();
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



