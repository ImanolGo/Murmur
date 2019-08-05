/*
 *  FloorManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 02/07/15.
 *
 */


#include "FluidVisual.h"
#include "FloorManager.h"


const int FloorManager::FLOOR_POS_CIRCLE_RADIUS = 20;

FloorManager::FloorManager(): m_kathakMinSize(10), m_kathakMaxSize(50), m_kathakLineWidth(20), m_invertedCoordinates(false)
{
    //Intentionaly left empty
}


FloorManager::~FloorManager()
{
    ofLogNotice() <<"FloorManager::Destructor" ;
}


void FloorManager::setup()
{
    Manager::setup();
    
    ofLogNotice() <<"FloorManager::initialized" ;
}


void FloorManager::draw()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(ofColor::white);
    
    ofNoFill();
    
    auto transformedPosition = m_floorPosition;
    transformedPosition.x *= ofGetWidth();
    transformedPosition.y *= ofGetHeight();
    
    ofDrawCircle(transformedPosition, FLOOR_POS_CIRCLE_RADIUS);
    ofDrawCircle(transformedPosition, FLOOR_POS_CIRCLE_RADIUS/10);
    //ofLogNotice() <<"FloorManager::draw << x:  " << transformedPosition << ", y: " << transformedPosition.y;
    
    ofPopStyle();
    ofPopMatrix();
}

void FloorManager::draw(const ofRectangle& area)
{
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(ofColor::white);
    
    ofNoFill();
    
    auto transformedPosition = m_floorPosition;
    transformedPosition.x *= area.width;
    transformedPosition.y *= area.height;
    
    ofDrawCircle(transformedPosition, FLOOR_POS_CIRCLE_RADIUS);
    ofDrawCircle(transformedPosition, FLOOR_POS_CIRCLE_RADIUS/10);
    //ofLogNotice() <<"FloorManager::draw << x:  " << transformedPosition << ", y: " << transformedPosition.y;
    
    ofPopStyle();
    ofPopMatrix();
}


void FloorManager::setPosition(const ofVec2f& position)
{
    m_floorPosition =  position - 0.5;
    m_floorPosition *= m_floorScale;
    m_floorPosition = m_floorPosition + 0.5 + m_floorOffset;
    
    if(m_invertedCoordinates){
        float x =  m_floorPosition.x;
        m_floorPosition.x = m_floorPosition.y;
        m_floorPosition.y = x;
    }
}


void FloorManager::setOffset(ofVec2f & offset)
{
    m_floorOffset = offset;
    
    if(m_invertedCoordinates){
        float x =  m_floorOffset.x;
        m_floorOffset.x = m_floorOffset.y;
        m_floorOffset.y = x;
    }
    
}

void FloorManager::setScale(ofVec2f & scale)
{
    m_floorScale = scale;
    
    if(m_invertedCoordinates){
        float x =  m_floorScale.x;
        m_floorScale.x = m_floorScale.y;
        m_floorScale.y = x;
    }
}

