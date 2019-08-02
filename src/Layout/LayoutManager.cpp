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


#include "LayoutManager.h"

const int LayoutManager::MARGIN = 30;
const int LayoutManager::PADDING = 10;


LayoutManager::LayoutManager(): Manager(), m_cropLeft(0), m_cropRight(0), m_cropTop(0), m_cropBottom(0)
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

    this->createTextVisuals();
    this->createImageVisuals();

    //this->addVisuals();
    
    ofLogNotice() <<"LayoutManager::initialized";

}


void LayoutManager::createTextVisuals()
{
    ///To implement in case we have text visuals
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
    
//    ofPushStyle();
//    ofSetColor(0, 0, 0);
//        ofRect(0,0,m_cropLeft,ofGetHeight());
//        ofRect(0,0,ofGetWidth(),m_cropTop);
//        ofRect(ofGetWidth()-m_cropRight,0, m_cropRight, ofGetHeight());
//        ofRect(0,ofGetHeight()-m_cropBottom,ofGetWidth(),m_cropBottom);
//    ofPopStyle();
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
//    if(!m_initialized){
//        return;
//    }
//    
//    this->resetWindowRects();
//    this->resetFbos();
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



