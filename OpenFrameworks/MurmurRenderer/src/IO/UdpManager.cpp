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
    
    this->setupUdpReceiver();
    this->setupHeaders();
    
    this->setupText();
    
    ofLogNotice() <<"UdpManager::initialized" ;
}

void UdpManager::setupUdpReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    
    m_udpConnection.Create(); //create the socket
    m_udpConnection.Bind(portReceive); //and bind to port
    m_udpConnection.SetNonBlocking(true);
    
     ofLogNotice() <<"UdpManager::setupUdpReceiver -> unable to connect to port  " << portReceive;
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
    
    m_trackingHeader.f1 = 0x10;
    m_trackingHeader.f2 = 0x41;
    m_trackingHeader.f3 = 0x37;
    m_trackingHeader.size = 4;
    m_trackingHeader.command = 't';
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
    //return;
    char udpMessage[UDP_MESSAGE_LENGHT];
    string message;
    string tempMessage;
    bool getNext = true;
    
    string text = ">>UdpManager::update -> " ;
    
    //ofLogNotice() <<"UdpManager::setupUdpReceiver -> update ";
    
    while (getNext) {
        
        int bytes = m_udpConnection.Receive(udpMessage,UDP_MESSAGE_LENGHT);
        //ofLogNotice() <<"UdpManager::setupUdpReceiver -> update : bytes = " << bytes;
        message=udpMessage;
        
        if (bytes==0){
            getNext = false;
        }
        else{
            this->parseMessage(udpMessage, bytes);
             // this->updateReceiveText(message);
        }
        
    }
    
    //ofLogNotice() <<">>UdpManager::update -> message: " << message;
  
    
    //AppManager::getInstance().getHandsManager().readHands(message.c_str());
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

void UdpManager::parseMessage(char * buffer, int size)
{
    if(this->isKinectMessage(buffer, size)){
        this->parseKinect(buffer, size);
    }
    else if(this->isHandsMessage(buffer, size)){
       AppManager::getInstance().getHandsManager().readHands(buffer, size);
    }
}

void UdpManager::parseKinect(char * buffer, int size)
{
    if(size< 4){
        return;
    }
    
    char command = buffer[3];
    //ofLogNotice() <<"UdpManager::parseKinect -> command  " << buffer[3];
    //this->printHex( buffer, size);
    if(command == m_contourHeader.command){
        this->parseContour( buffer, size);
    }
    else if(command == m_audioHeader.command){
        this->parseAudio( buffer, size);
    }
    else if(command == m_trackingHeader.command){
        this->parseTracking( buffer, size);
    }
}

void UdpManager::parseAudio(char * buffer, int size)
{
    if(size< 10){
        return;
    }
    
    float f;
    char b[] = {buffer[6], buffer[7], buffer[8], buffer[9]};
    
    memcpy(&f, &b, sizeof(f));
    AppManager::getInstance().getAudioManager().setAudioMax(f);
   // ofLogNotice() <<"UdpManager::parseAudio -> audio max  " << f;
}

void UdpManager::parseTracking(char * buffer, int size)
{
    if(size< 14){
        return;
    }
    
    ofVec2f pos;
    int index = 6;
    float f;
    char b[4];
    
    // x coordinate
    b[0] = buffer[index++]; b[1] = buffer[index++];b[2] = buffer[index++]; b[3] = buffer[index++];
    memcpy(&f, &b, sizeof(f));
    pos.x = f;
    
    // y coordinate
    b[0] = buffer[index++]; b[1] = buffer[index++];b[2] = buffer[index++]; b[3] = buffer[index++];
    memcpy(&f, &b, sizeof(f));
    pos.y = f;
    
    AppManager::getInstance().getFloorManager().setPosition(pos);
    
}


void UdpManager::parseContour(char * buffer, int size)
{
   
    if(!this->isContourMessage(buffer, size)){
        return;
    }
    
    unsigned int index = 6;
    char b[2];
    
    //Get data width
    unsigned short width;
    b[0] = buffer[index++]; b[1] = buffer[index++];
    memcpy(&width, &b, sizeof(width));
    
    //Get data height
    unsigned short height;
    b[0] = buffer[index++]; b[1] = buffer[index++];
    memcpy(&height, &b, sizeof(height));
    
    //Get num contours height
    unsigned short num_contours;
    b[0] = buffer[index++]; b[1] = buffer[index++];
    memcpy(&num_contours, &b, sizeof(num_contours));
    
    //ofLogNotice() <<"UdpManager::parseContour -> width  " << width;
    //ofLogNotice() <<"UdpManager::parseContour -> height  " << height;
   // ofLogNotice() <<"UdpManager::parseContour -> num contours  " << num_contours;
    
    AppManager::getInstance().getContourManager().resetContours();
    
    for(unsigned short i = 0; i< num_contours; i++)
    {
        
        if(index+1>=size){
            return;
        }
        
        unsigned short contour_size;
        b[0] = buffer[index++]; b[1] = buffer[index++];
        memcpy(&contour_size, &b, sizeof(contour_size));
        //ofLogNotice() <<"UdpManager::parseContour -> contour size  " << contour_size;
        
        
        vector<float> contourPoints;
        // std::cout<< "points: ";
         for(unsigned short j = 0; j< contour_size; j++)
         {
             if(index+3>=size){
                 return;
             }
             
             unsigned short coordinate;
             b[0] = buffer[index++]; b[1] = buffer[index++];
             memcpy(&coordinate, &b, sizeof(coordinate));
             float x = float(coordinate) / width;
             contourPoints.push_back(x); //x
             //std::cout<< " " << x;
             
             b[0] = buffer[index++]; b[1] = buffer[index++];
             memcpy(&coordinate, &b, sizeof(coordinate));
             float y = float(coordinate) / height;
             contourPoints.push_back(y); //y
             //std::cout<< " " << y;
             
         }
        
        //std::cout<< " " << std::endl;
       
        AppManager::getInstance().getContourManager().setContour(contourPoints);
        
    }
}


bool UdpManager::isContourMessage(char * buffer, int size)
{
    unsigned int header_size = 6;
    if(size< header_size){
        return false;
    }
    
    //Get data size
    unsigned short s;
    char b[] = {buffer[4], buffer[5]};
    
    memcpy(&s, &b, sizeof(s));
    // ofLogNotice() <<"UdpManager::parseContour -> num bytes total " << size;
    //ofLogNotice() <<"UdpManager::parseContour -> num bytes data " << s;
    
    //Check data size
    int data_size = size - header_size;
    if(data_size != s){
        return false;
    }
    
     return true;
}

bool UdpManager::isKinectMessage(char * buffer, int size)
{
    if(size< 3){
        return false;
    }

    if(buffer[0] != m_contourHeader.f1  && buffer[1] != m_contourHeader.f2  && buffer[2] != m_contourHeader.f3 ){
       // ofLogNotice() <<"UdpManager::isMessage -> FALSE ";
        return false;
    }
    
   // ofLogNotice() <<"UdpManager::isMessage -> TRUE ";
    return true;
}

bool UdpManager::isHandsMessage(char * buffer, int size)
{
    if(size< 2){
        return false;
    }
    
    if(buffer[0] !='X' ){ //Beginning of message
        // ofLogNotice() <<"UdpManager::isMessage -> FALSE ";
        return false;
    }
    
    
   // ofLogNotice() <<"UdpManager::isMessage -> TRUE ";
    return true;
}


void UdpManager::printHex(char * buffer, int size)
{
    std::stringstream ss;
    for(int i=0; i<size; ++i){
        ss << std::hex << (int)buffer[i] << " ";
    }
    std::string mystr = ss.str();
    
    ofLogNotice() <<"UdpManager::printHex ->  hex: " << mystr;
}






