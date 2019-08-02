/*
 *  OscManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxOsc.h"
#include "TextVisual.h"
#include "RectangleVisual.h"

//========================== class OscManager =======================================
//==============================================================================
/** \class OscManager OscManager.h
 *    \brief class for managing the OSC events
 *    \details It reads all the OSC events and create the proper application events
 */


class OscManager: public Manager
{
    
public:
    //! Constructor
    OscManager();
    
    //! Destructor
    virtual ~OscManager();
    
    //! setups the manager
    void setup();
    
    //! updates the manager
    void update();
    
    //! draw the manager
    void draw();
    
    // Send Osc message to the floor tracking software
    void sendMessageToFloorTracking(ofxOscMessage& message);
    
    // Send Osc message to the contour tracking software
    void sendMessageToContourTracking(ofxOscMessage& message);
    
    // Send Osc message to Unity 3D
    void sendMessageToUnity(ofxOscMessage& message);
    
    //Send Paper Thrower Speed
    void onChangePaperThrowerSpeed(int& value);
    
    //Send Paper Thrower Fire command
    void onFirePaperThrower();
    
    //Send Paper Reset Background command
    void onResetTopBackground();
    
    ofPtr<RectangleVisual> getBoundingBox() const {return m_boundingBox;}
    
private:
    
    //! sets upt the osc receiver
    void setupOscReceiver();
    
    //! sets upt the osc sender
    void setupOscSender();
    
    //! setups the text visuals
    void setupText();
    
    //! updates the sending information text visuals
    void updateSendFloorText();
    
    //! updates the sending information text visuals
    void updateSendContourText();
    
    //! updates receiving information text visuals
    void updateReceiveText();
    
    //! gets string formatted OSC message
    string getMessageAsString(const ofxOscMessage& m) const;
    
    
private:
    
    ofxOscReceiver m_oscReceiver;          ///< OSC receiver class
    ofxOscSender   m_oscFloorTrackingSender;            ///< OSC sender class
    ofxOscSender   m_oscContourTrackingSender;            ///< OSC sender class
    ofxOscSender   m_oscSenderUnity;       ///< OSC sender connected with Unity 3D
    ofxOscMessage  m_latestOscMessage;    ///< latest OSC message
    
    ofPtr<TextVisual>          m_sendingInformation;
    ofPtr<TextVisual>          m_receivingInformation;
    ofPtr<RectangleVisual>     m_boundingBox;
    
    int                   m_numberOfContours;
    
};

