/*
 *  UdpManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 18/09/19.
 *
 */




#include "UdpManager.h"
#include "AppManager.h"

const int UdpManager::UDP_MESSAGE_LENGHT = 10000;

UdpManager::UdpManager(): Manager()
{
    //Intentionally left empty
}

UdpManager::~UdpManager()
{
    ofLogNotice() << "UdpManager::destructor";
}


//--------------------------------------------------------------

void UdpManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupHeaders();
    this->setupUdpConnection();
    this->setupText();

    ofLogNotice() <<"UdpManager::initialized" ;
}


void UdpManager::setupHeaders()
{
    m_trackingHeader.f1 = 0x10;
    m_trackingHeader.f2 = 0x41;
    m_trackingHeader.f3 = 0x37;
    m_trackingHeader.size = 8;
    m_trackingHeader.command = 't';
}

void UdpManager::setupUdpConnection()
{

    string ip = AppManager::getInstance().getSettingsManager().getIpAddress();
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    
    //create the socket and set to send to 127.0.0.1:11999
    ofxUDPSettings settings;
    settings.sendTo(ip, portSend);
    settings.blocking = false;
    
    m_udpConnection.Setup(settings);
    
    ofLogNotice() <<"UdpManager::setupUdpSender -> sending to IP " << ip <<" to port " << portSend;
    
}


void UdpManager::setupText()
{
    ofVec3f position;
    ofVec2f positionTrackingVisual = AppManager::getInstance().getTrackingManager().getPosition();
    int width = AppManager::getInstance().getTrackingManager().getWidth();
    int fontSize = 12;
    position.x = positionTrackingVisual.x;
    position.y = positionTrackingVisual.y +  AppManager::getInstance().getTrackingManager().getWidth() + LayoutManager::MARGIN;
    int height = fontSize*3;
    
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();
    string text = ">> UDP sending -> Host: " + host + ", Port: " + ofToString(portSend);
    
    m_sendingInformation =  TextVisual(position, width, height);
    m_sendingInformation.setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_sendingInformation.setColor(ofColor::white);
    m_sendingInformation.setLineHeight(2.5);
    
    text = "";
    position.y += 2*fontSize;
    m_sendingInformation1 =  TextVisual(position, width, height);
    m_sendingInformation1.setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_sendingInformation1.setColor(ofColor::white);
    m_sendingInformation1.setLineHeight(2.5);
    
}

void UdpManager::update()
{
    //
    
}

void UdpManager::sendPosition(const ofPoint& pos)
{
    string message="";
    message+= this->getTrackingHeader();
    message+= this->getTrackingData(pos);
    m_udpConnection.Send(message.c_str(),message.length());
    this->updateSendText(pos);
}

void UdpManager::updateSendText(const ofPoint& pos)
{
    string text = "Position: x = " + ofToString(pos.x) + ", y = " + ofToString(pos.y);
    m_sendingInformation1.setText(text);
}


string UdpManager::getTrackingHeader()
{
    string message="";
   
    message+= m_trackingHeader.f1;
    message+= m_trackingHeader.f2;
    message+= m_trackingHeader.f3;
    message+= m_trackingHeader.command;
    
    unsigned char * s = (unsigned char*)& m_trackingHeader.size;
    message+= s[0];  message+= s[1];
    
    return message;
}

void UdpManager::draw()
{
    m_sendingInformation.draw();
    m_sendingInformation1.draw();
}


string UdpManager::getTrackingData(const ofPoint& pos)
{
    string message="";
    Float32 v = pos.x;
    unsigned char * s = (unsigned char*)& v;
    message+= s[0];  message+= s[1]; message+= s[2];  message+= s[3];
    
    v = pos.y;
    s = (unsigned char*)& v;
    message+= s[0];  message+= s[1]; message+= s[2];  message+= s[3];
    
    return message;
}




