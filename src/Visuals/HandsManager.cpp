/*
 *  HandsManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 01/07/15.
 *
 */

#include "FluidVisual.h"
#include "HandsManager.h"

const int HandsManager::HANDS_CIRCLE_RADIUS = 20;

HandsManager::HandsManager(): m_handsOffset(ofVec2f(0,0)), m_handsScale(ofVec2f(1,1))
{
    //Intentionaly left empty
}


HandsManager::~HandsManager()
{
    ofLogNotice() <<"HandsManager::Destructor" ;
}


void HandsManager::setup()
{
    Manager::setup();
    
    m_handsScale = ofVec2f(1.0,1.0);
    ofLogNotice() <<"HandsManager::initialized" ;
    
    this->setupHandsRectangleSpace();
    
}


void HandsManager::update()
{
    m_hands.clear();
}

void HandsManager::draw()
{
    ofPushMatrix();
    ofPushStyle();
   
    ofSetColor(ofColor::white);
    
        ofNoFill();
        ofRect(m_handsRectangleSpace.position.x*ofGetWidth(), m_handsRectangleSpace.position.y*ofGetHeight(), m_handsRectangleSpace.width*ofGetWidth(), m_handsRectangleSpace.height*ofGetHeight() );
    
        for (auto hand: m_hands){
            
            auto transformedHand = hand;
            transformedHand.x *= ofGetWidth();
            transformedHand.y *= ofGetHeight();
            
            
            ofCircle(transformedHand, HANDS_CIRCLE_RADIUS);
            ofCircle(transformedHand, HANDS_CIRCLE_RADIUS/10);
            //ofLogNotice() <<"HandsManager::readHands << x:  " << hand.x << ", y: " << hand.y;
            
        }
    ofPopStyle();
    ofPopMatrix();
}


void HandsManager::setupHandsRectangleSpace()
{
    m_handsRectangleSpace = ofRectangle(0.0,0.0,1.0,1.0);
}

void HandsManager::readHands(char const* data)
{
    
    if(!m_handsOn){
        return;
    }
    
    //Formatting the message: 'X' <len(c8)> [<xpos(f32),ypos(f32)>] 'x'
    
    
    //ofLogNotice() <<"HandsManager::readHands << headbyte -> " << data[0];
    
    char const* p = data;
    
    char headerByte;
    p = extract(p, headerByte); // p contains next position to read
    
    //ofLogNotice() <<"HandsManager::readHands << headbyte -> " << headerByte;
    
    if(headerByte == 'X') //Beginning of message
    {
        m_hands.clear();
        
        char charNumberOfHands;
        p = extract(p, charNumberOfHands); // p contains next position to read
        
        int numberOfHands = charNumberOfHands;
        
        //ofLogNotice() <<"HandsManager::readHands << numberOfHands -> " << numberOfHands;
        
        for(int i = 0; i < numberOfHands; i++) //Extract all the hands
        {
            Float32 x, y;
            p = extract(p, x); // p contains next position to read
            
            //x = htonl(x);
            p = extract(p, y); // p contains next position to read
            //y = ntohs(y);
            
            ofLogNotice() <<"HandsManager::readHands << y -> " << y;
            ofVec2f hand = ofVec2f(x,y) - 0.5;
            hand *= m_handsScale;
            hand = hand + 0.5 + m_handsOffset;
            
            hand.x = ofClamp(hand.x, 0, 1);
            hand.y = ofClamp(hand.y, 0, 1);
            
            //ofLogNotice() <<"HandsManager::readHands << y -> " << hand.y;
            //ofLogNotice() <<"HandsManager::readHands << x -> " << hand.x;
            
            m_hands.push_back(hand);
            
        }
        
        //ofLogNotice() <<"HandsManager::readHands << x:  " << m_hands[0].x << ", y: " << m_hands[0].y;
        
        char tailByte;
        p = extract(p, tailByte); // p contains next position to read
        
        //ofLogNotice() <<"HandsManager::readHands << tailByte -> " << tailByte;
        
        if(tailByte == 'x') //Message correctly formatted
        {
            //ofLogNotice() <<"HandsManager::readHands << Hands correctly parsed";
            return;
        }
        
    }
    
}


/**
 * General routing to extract aligned integral types
 * from the UDP packet.
 *
 * @param data Pointer into the UDP packet data
 * @param type Integral type to extract
 *
 * @return data pointer advanced to next position after extracted integral.
 */
template<typename Type>
char const* HandsManager::extract(char const* data, Type& type)
{
    // This union will ensure the integral data type is correctly aligned
    union tx_t
    {
        char cdata[sizeof(Type)];
        Type tdata;
    } tx;
    
    for(size_t i(0); i < sizeof(Type); ++i)
        tx.cdata[i] = data[i];
    
    type = tx.tdata;
    
    return data + sizeof(Type);
}

/**
 * If strings are null terminated in the buffer then this could be used to extract them.
 *
 * @param data Pointer into the UDP packet data
 * @param s std::string type to extract
 *
 * @return data pointer advanced to next position after extracted std::string.
 */
char const* HandsManager::extract(char const* data, std::string& s)
{
    s.assign((char const*)data, std::strlen((char const*)data));
    return data + s.size();
}


void HandsManager::setOffset(ofVec2f & offset)
{
    m_handsOffset = offset;
    m_handsRectangleSpace.setFromCenter(m_handsOffset.x+0.5, m_handsOffset.y+0.5, m_handsScale.x, m_handsScale.y);
}

void HandsManager::setScale(ofVec2f & scale)
{
    m_handsScale = scale;
    m_handsRectangleSpace.setFromCenter(m_handsOffset.x+0.5, m_handsOffset.y+0.5, m_handsScale.x, m_handsScale.y);
}
