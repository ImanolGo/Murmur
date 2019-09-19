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

    ofLogNotice() <<"UdpManager::initialized" ;
}


void UdpManager::setupHeaders()
{
    m_contourHeader.f1 = 0x10;
    m_contourHeader.f2 = 0x41;
    m_contourHeader.f3 = 0x37;
    m_contourHeader.size = 0;
    m_contourHeader.command = 'c';
    
    m_audioHeader.f1 = 0x10;
    m_audioHeader.f2 = 0x41;
    m_audioHeader.f3 = 0x37;
    m_audioHeader.size = 4;
    m_audioHeader.command = 'a';
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


void UdpManager::update()
{
    //
    
}

void UdpManager::sendContour(const vector<ofPolyline>& contours)
{
    string message="";
    message+= this->getContourHeader(contours);
    message+= this->getContourData(contours);
    m_udpConnection.Send(message.c_str(),message.length());
}

void UdpManager::sendAudioMax(float value)
{
    string message="";
    message+= this->getAudioHeader();
    message+= this->getAudioData(value);
    m_udpConnection.Send(message.c_str(),message.length());
}

string UdpManager::getContourHeader(const vector<ofPolyline>& contours)
{
    string message="";
    m_contourHeader.size  = getContourDataSize(contours);
    message+= m_contourHeader.f1;
    message+= m_contourHeader.f2;
    message+= m_contourHeader.f3;
    message+= m_contourHeader.command;
   
    unsigned char * s = (unsigned char*)& m_contourHeader.size;
    message+= s[0];  message+= s[1];
    
    return message;
}

string UdpManager::getContourData(const vector<ofPolyline>& contours)
{
    string message="";
    unsigned short num_blobs = contours.size();
    unsigned char * s = (unsigned char*)& num_blobs;
    message+= s[0];  message+= s[1];
    
    for(auto& contour: contours)
    {
        unsigned short size = contour.size();
        s = (unsigned char*)& size;
        message+= s[0];  message+= s[1];
        
        for (ofPoint blobPoint : contour.getVertices())
        {
            Float32 x = blobPoint.x / TrackingManager::DEPTH_CAMERA_WIDTH;
            Float32 y = blobPoint.y / TrackingManager::DEPTH_CAMERA_HEIGHT;
            
            s = (unsigned char*)& x;
            message+= s[0];  message+= s[1]; message+= s[2];  message+= s[3];
            
            s = (unsigned char*)& y;
            message+= s[0];  message+= s[1]; message+= s[2];  message+= s[3];
            
        }
    }
    
    return message;
}


string UdpManager::getAudioHeader()
{
    string message="";
   
    message+= m_audioHeader.f1;
    message+= m_audioHeader.f2;
    message+= m_audioHeader.f3;
    message+= m_audioHeader.command;
    
    unsigned char * s = (unsigned char*)& m_audioHeader.size;
    message+= s[0];  message+= s[1];
    
    return message;
}

string UdpManager::getAudioData(float value)
{
    string message="";
    Float32 v = value;
    
    unsigned char * s = (unsigned char*)& v;
    message+= s[0];  message+= s[1]; message+= s[2];  message+= s[3];
    
    return message;
}


unsigned short UdpManager::getContourDataSize(const vector<ofPolyline>& contours)
{
    unsigned short size = 0;
    size+=2; // Num_blobs = 2 bytes
    size += (2*contours.size()); //2 bytes * num_contours
    
    for(auto& contour: contours){
        size+= 4*2*contour.size(); //4 bytes per coordinate (x,y)
    }
    
    return size;
}

