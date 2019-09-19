/*
 *  UdpManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 01/07/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxNetwork.h"
#include "TextVisual.h"

//========================== class UdpManager =======================================
//==============================================================================
/** \class UdpManager UdpManager.h
 *	\brief class for managing the UDP messages
 *	\details It reads all the UDP messages and create the proper application events
 */


class UdpManager: public Manager
{

    static const int UDP_MESSAGE_LENGHT; ///Defines the Udp"s message length
    
    struct udp_header {
        unsigned char f1;
        unsigned char f2;
        unsigned char f3;
        unsigned char command;
        short size;
    };
    
public:
    //! Constructor
    UdpManager();

    //! Destructor
    virtual ~UdpManager();

    //! setups the manager
    void setup();

    //! updates the manager
    void update();
    

private:
    
    //! sets up the udp receiver
    void setupUdpReceiver();
    
    void setupHeaders();
    
    //! setups the text visuals
    void setupText();
    
    //! updates receiving information text visuals
    void updateReceiveText(const string& message);
    
    bool isKinectMessage(char * buffer, int size);
    
    bool isHandsMessage(char * buffer, int size);
    
    bool isContourMessage(char * buffer, int size);
    
    bool isTrackingMessage(char * buffer, int size);
    
    void parseMessage(char * buffer, int size);
    
    void parseKinect(char * buffer, int size);
    
    void parseContour(char * buffer, int size);
    
    void parseContourData(char * buffer, int size);
    
    void parseAudio(char * buffer, int size);
    
    void parseTracking(char * buffer, int size);
    
    void printHex(char * buffer, int size);
    
    
 private:
    
 
     ofxUDPManager          m_udpConnection;        ///< ofxUdpManager  class
     ofPtr<TextVisual>      m_udpReceiveTextFont;
     ofPtr<TextVisual>       m_udpReceiveMessageFont;
    
    udp_header    m_contourHeader;
    udp_header    m_audioHeader;
    udp_header    m_trackingHeader;
    
};

