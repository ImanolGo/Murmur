/*
 *  UdpManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 18/09/19.
 *
 */



#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "TextVisual.h"
#include "Manager.h"


//========================== class UdpManager =======================================
//==============================================================================
/** \class UdpManager UdpManager.h
 *    \brief class for managing the udp connection
 *    \details It writes and send udp data
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
    
    //! setups the udp manager
    void setup();
    
    //! updates the udp manager
    void update();
    
    //! draw the udp manager
    void draw();
    
    
    //! send position
    void sendPosition(const ofPoint& pos);
    
    
private:
    
    void setupUdpConnection();
    
    void setupHeaders();
    
    //! setups the text visuals
    void setupText();
    
    //! updates the sending information text visuals
    void updateSendText(const ofPoint& pos);
    
    string getTrackingHeader();
    
    string getTrackingData(const ofPoint& pos);
    
private:
    
    ofxUDPManager m_udpConnection;
    udp_header    m_trackingHeader;
    
    TextVisual     m_sendingInformation, m_sendingInformation1;
};

