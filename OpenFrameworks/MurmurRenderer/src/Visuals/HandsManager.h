/*
 *  HandsManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 01/07/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"

//========================== class HandsManager ==============================
//============================================================================
/** \class HandsManager HandsManager.h
 *	\brief Class managing the hands information
 *	\details It reads the hands given from the UDP messages and puts them into a texture
 */


class HandsManager: public Manager
 {
     
     static const int HANDS_CIRCLE_RADIUS;

public:
    
    //! Constructor
    HandsManager();
    
    //! Destructor
    ~HandsManager();
    
    //! Setup the Hands Manager
    void setup();
    
    //! Update Hands the Hands Manager
    void update();
     
    //! Draw the Hands Manager
    void draw();
     
    //! parses the hands information coming from the laser ranger
    void readHands(char const* udpMessage);
     
     //! parses the hands information coming from the laser ranger
     void readHands2(char const* udpMessage);
     
    const vector< ofVec2f >& getHands() const {return m_hands;}
     
    void setOffset(ofVec2f & offset);
     
    void setScale(ofVec2f & scale);
     
    void setHandsOn(bool & value) {m_handsOn = value;}
     
    void setFadeTime(float & value) {m_fadeTime = value;}
     
    void setSize(float & value) {m_size = value;}
     
    float getFadeTime() const {return m_fadeTime;}
     
    float getSize() const {return m_size;}
    
     
private:
     
     void setupHandsRectangleSpace();
     
     //! General routing to extract aligned integral types from the UDP packet.
     template<typename Type>
     char const* extract(char const* data, Type& type);
     
     //! If strings are null terminated in the buffer then this could be used to extract them.
     char const* extract(char const* data, std::string& s);

    
private:
     
    
    typedef vector< ofVec2f > HandVector;
     
    HandVector          m_hands; ///< Vector holding the hands information
     
    ofVec2f             m_handsOffset;
    ofVec2f             m_handsScale;
    ofRectangle         m_handsRectangleSpace;
    float               m_fadeTime;
    float               m_size;
     
    bool                m_handsOn;
     
};



