/*
 *  UdpManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 01/07/15.
 *
 */


#include "UdpManager.h"
#include "SettingsManager.h"
#include "HandsManager.h"
#include "WindowSettingsManager.h"
#include "AppManager.h"


const int UdpManager::UDP_MESSAGE_LENGHT = 1000;

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
    
    this->setupUdpReceiver();
    this->setupText();
    
    ofLogNotice() <<"UdpManager::initialized" ;
}

void UdpManager::setupUdpReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    ofLogNotice() <<"UdpManager::setupUdpReceiver -> listening for udp messages on port  " << portReceive;
    
    m_udpConnection.Create(); //create the socket
    m_udpConnection.Bind(portReceive); //and bind to port
    m_udpConnection.SetNonBlocking(true);
}

void UdpManager::setupText()
{
    auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings(0);
   // auto guiManager = AppManager::getInstance().getGuiManager();
    
    int width =  (windowSettings.getWidth() - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5 - LayoutManager::MARGIN;
    int fontSize = 12;
    int height = fontSize*3;
    
    ofVec3f position;
    
    //position.x = GuiManager::GUI_WIDTH + 2*LayoutManager::MARGIN;
    position.y = 9*LayoutManager::MARGIN + windowSettings.getHeight()*0.5;
    position.x = 2.5*LayoutManager::MARGIN + GuiManager::GUI_WIDTH;
    
    int porReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    string text = ">> UDP receiving -> Port: " + ofToString(porReceive);
    
    m_udpReceiveTextFont =  ofPtr<TextVisual> (new TextVisual(position, width, height));
    m_udpReceiveTextFont->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_udpReceiveTextFont->setColor(ofColor::white);
    m_udpReceiveTextFont->setLineHeight(2.5);
    
    position.y += LayoutManager::MARGIN + fontSize;
    m_udpReceiveMessageFont =  ofPtr<TextVisual> (new TextVisual(position, width, height));
    m_udpReceiveMessageFont->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_udpReceiveMessageFont->setColor(ofColor::white);
    m_udpReceiveMessageFont->setLineHeight(2.5);
    
    AppManager::getInstance().getViewManager().addOverlay(m_udpReceiveTextFont);
    AppManager::getInstance().getViewManager().addOverlay(m_udpReceiveMessageFont);
    
    
    position.y = 8.5*LayoutManager::MARGIN + windowSettings.getHeight()*0.5;
    position.x -= 0.5*LayoutManager::MARGIN;
    width += LayoutManager::MARGIN;
    height = 4*LayoutManager::MARGIN;
    
    ofPtr<RectangleVisual> rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    ofColor color(60,60,60);
    rectangleVisual->setColor(color);
    
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);
}


void UdpManager::update()
{
    char udpMessage[UDP_MESSAGE_LENGHT];
    string message;
    string tempMessage;
    bool getNext = true;
    
    string text = ">>UdpManager::update -> " ;
    
    while (getNext) {
        m_udpConnection.Receive(udpMessage,UDP_MESSAGE_LENGHT);
        tempMessage=udpMessage;
        
        if (tempMessage==""){
            getNext = false;
        }
        else{
            message = tempMessage;
        }
        
    }
    
    //ofLogNotice() <<">>UdpManager::update -> message: " << message;
    this->updateReceiveText(message);
    
    AppManager::getInstance().getHandsManager().readHands(message.c_str());
    //AppManager::getInstance().getHandsManager().readHands2(message.c_str());
}


void UdpManager::updateReceiveText(const string& message)
{
    int porReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    string text = ">> UDP receiving -> Port: " + ofToString(porReceive) ;
    //text += "   " + message;
    m_udpReceiveTextFont->setText(text);
    
    m_udpReceiveMessageFont->setText("   " + message);

}









