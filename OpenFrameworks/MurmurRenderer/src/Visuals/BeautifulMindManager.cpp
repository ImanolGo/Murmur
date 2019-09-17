/*
 *  BeautifulMindManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 04/07/15.
 *
 */

#include "AppManager.h"
#include "BeautifulMindManager.h"

BeautifulMindManager::BeautifulMindManager(): m_handsOffset(ofVec2f(0,0)), m_handsScale(ofVec2f(1,1)), m_floorOn(false), m_calibrationOn(false), m_floorSpeed(1.0)
{
    //Intentionaly left empty
}


BeautifulMindManager::~BeautifulMindManager()
{
    ofLogNotice() <<"BeautifulMindManager::Destructor" ;
}


void BeautifulMindManager::setup()
{
    Manager::setup();
    
    ofLogNotice() <<"BeautifulMindManager::initialized" ;
    
    this->setupRectangleSpace();
    
}


void BeautifulMindManager::draw()
{
    ofPushMatrix();
    ofPushStyle();
   
    ofSetColor(ofColor::white);
    
        ofNoFill();
        ofDrawRectangle(m_rectangleSpace.position.x*ofGetWidth(), m_rectangleSpace.position.y*ofGetHeight(), m_rectangleSpace.width*ofGetWidth(), m_rectangleSpace.height*ofGetHeight() );
    
    ofPopStyle();
    ofPopMatrix();
}


void BeautifulMindManager::setupRectangleSpace()
{
    m_rectangleSpace = ofRectangle(0.0,0.0,1.0,1.0);
}



void BeautifulMindManager::setOffset(ofVec2f & offset)
{
    m_handsOffset = offset;
    m_rectangleSpace.setFromCenter(m_handsOffset.x+0.5, m_handsOffset.y+0.5, m_handsScale.x, m_handsScale.y);
}

void BeautifulMindManager::setScale(ofVec2f & scale)
{
    m_handsScale = scale;
    m_rectangleSpace.setFromCenter(m_handsOffset.x+0.5, m_handsOffset.y+0.5, m_handsScale.x, m_handsScale.y);
}
