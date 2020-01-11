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
    this->updateSendText(value);
}

void UdpManager::updateSendText(float value)
{
    string text = "Audio Max: " + ofToString(value);
    m_sendingInformation1.setText(text);
}

void UdpManager::draw()
{
    m_sendingInformation.draw();
    m_sendingInformation1.draw();
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
    unsigned short blobs_width = TrackingManager::DEPTH_CAMERA_WIDTH;
    unsigned char * s = (unsigned char*)& blobs_width;
    message+= s[0];  message+= s[1];
    unsigned short blobs_height = TrackingManager::DEPTH_CAMERA_HEIGHT;
    s = (unsigned char*)& blobs_height;
    message+= s[0];  message+= s[1];
    
    unsigned short num_contours = contours.size();
    s = (unsigned char*)& num_contours;
    message+= s[0];  message+= s[1];
    
    for(auto& contour: contours)
    {
        unsigned short size = contour.size();
        s = (unsigned char*)& size;
        message+= s[0];  message+= s[1];
        
        for (ofPoint blobPoint : contour.getVertices())
        {
            unsigned short x = blobPoint.x;
            unsigned short y = blobPoint.y;
            
            s = (unsigned char*)& x;
            message+= s[0];  message+= s[1];
            
            s = (unsigned char*)& y;
            message+= s[0];  message+= s[1];
            
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
    size+= sizeof(unsigned short) ; // Num_blobs = 2 bytes
    size+=2*sizeof(unsigned short); // W x H
    size += (sizeof(unsigned short)*contours.size()); //2 bytes * num_contours
    
    for(auto& contour: contours){
        size+= sizeof(unsigned short)*2*contour.size(); //4 bytes per coordinate (x,y)
    }
    
    return size;
}

