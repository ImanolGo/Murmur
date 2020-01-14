/*
 *  SettingsManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */


#include "ofMain.h"

#include "SettingsManager.h"


const string SettingsManager::APPLICATION_SETTINGS_FILE_NAME = "xmls/ApplicationSettings.xml";
const string SettingsManager::LOCALHOST = "127.0.0.1";
const int SettingsManager::MAX_NUM_WINDOWS = 3;



SettingsManager::SettingsManager(): Manager(), m_portUdpReceive(0), m_portOscReceive(0), m_portOscFloor(0), m_portOscUnity(0), m_ipFloor(LOCALHOST), m_ipContour(LOCALHOST)
{
    //Intentionally left empty
}


SettingsManager::~SettingsManager()
{
    ofLogNotice() <<"SettingsManager::Destructor" ;
}


void SettingsManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();

    if(this->loadSettingsFile()){
        this->loadAllSettings();
    }
    
    ofLogNotice() <<"SettingsManager::initialized" ;
}

void SettingsManager::loadAllSettings()
{
    this->setWindowProperties();
    this->setNetworkProperties();
    this->loadTextureSettings();
    this->loadColors();
}

bool SettingsManager::loadSettingsFile()
{

    //string path =  ofToDataPath(APPLICATION_SETTINGS_FILE_NAME,true);
    string path = APPLICATION_SETTINGS_FILE_NAME;
    if(!m_xml.load(path)){
        ofLogNotice() <<"SettingsManager::loadSettingsFile-> unable to load file: " << path ;
        return false;
    }
    
    ofLogNotice() <<"SettingsManager::loadSettingsFile->  successfully loaded " << path ;
    return true;
}


void SettingsManager::setWindowProperties()
{
    string path = "//of_settings/window";
    auto xml = m_xml.findFirst(path);
    if(xml)
    {
        string title = xml.getAttribute("title").getValue();
        
        int x = xml.getAttribute("x").getIntValue();
        int y = xml.getAttribute("y").getIntValue();
        bool fullscreen = xml.getAttribute("fullscreen").getBoolValue();
        float width  = ofGetScreenWidth();
        float height = ofGetScreenHeight();
        
        //ofSetFullscreen(fullscreen);
        //ofSetWindowShape(width, height);
        if(!fullscreen){
            ofSetWindowPosition(x,y);
        }
        ofSetWindowTitle(title);
        
        ofLogNotice() <<"SettingsManager::setWindowProperties->  successfully loaded the window settings" ;
        ofLogNotice() <<"SettingsManager::setWindowProperties->  title = "<< title<<", width = " << width <<", height = "
        <<height <<", x = "<<x<<", y = "<<y;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setWindowProperties->  path not found: " << path ;
}

void SettingsManager::setNetworkProperties()
{
    string path = "//of_settings/network";
    auto xml = m_xml.findFirst(path);
    if(xml) {
        
        m_ipContour  = xml.getAttribute("ipContour").getValue();
        m_ipFloor  = xml.getAttribute("ipFloor").getValue();
        m_portUdpReceive = xml.getAttribute("portUdpReceive").getIntValue();
        m_portOscFloor = xml.getAttribute("portOscFloor").getIntValue();
        m_portOscContour = xml.getAttribute("portOscContour").getIntValue();
        m_portOscUnity = xml.getAttribute("m_portOscUnity").getIntValue();
        m_portOscReceive = xml.getAttribute("portOscReceive").getIntValue();
        m_portUdpReceive = xml.getAttribute("portUdpReceive").getIntValue();
        m_portOscUnity = xml.getAttribute("portOscUnity").getIntValue();
        
        ofLogNotice() <<"SettingsManager::setNetworkProperties->  successfully loaded the network settings" ;
        ofLogNotice() <<"SettingsManager::setNetworkProperties->  receive UDP port = " << m_portUdpReceive << ". receive OSC port = "<< m_portOscReceive<<", send OSC port = " << m_portOscFloor<<", host = "
        <<m_ipFloor << ", send OSC Unity = "<< m_portOscUnity;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setNetworkProperties->  path not found: " << path ;
    
}


void SettingsManager::loadColors()
{
    string path = "//colors/color";
    auto colorsXml = m_xml.find(path);
    if(!colorsXml.empty()) {
        
        for(auto & colorXml: colorsXml)
        {
            int r = colorXml.getAttribute("r").getIntValue();
            int g = colorXml.getAttribute("g").getIntValue();
            int b = colorXml.getAttribute("b").getIntValue();
            int a = colorXml.getAttribute("a").getIntValue();
            string name =  colorXml.getAttribute("name").getValue();;
            
            m_colors[name] =  ofColor(r,g,b,a);
            
            ofLogNotice() <<"SettingsManager::loadColors->  color = " << name <<", r = " << r
            <<", g = "<< g << ", b = " << b << ", a = " << a ;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadColors->  successfully loaded the applications colors" ;
        return;
    }
    
    
    ofLogNotice() <<"SettingsManager::loadColors->  path not found: " << path ;
}


void SettingsManager::loadTextureSettings()
{
    string path = "//textures/texture";
    auto texturesXml = m_xml.find(path);
    if(!texturesXml.empty()) {
        
        for(auto & textureXml: texturesXml)
        {
            string path =  textureXml.getAttribute("path").getValue();
            string name =  textureXml.getAttribute("name").getValue();
            
            m_texturesPath[name] = path;
            
            
            ofLogNotice() <<"SettingsManager::loadTextureSettings->  texture = " << name
            <<", path = "<< path;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadTextureSettings->  successfully loaded the resource settings" ;
        return;
    }
    
    
    ofLogNotice() <<"SettingsManager::loadTextureSettings->  path not found: " << path ;
}

ofColor SettingsManager::getColor(const string& colorName)
{
    ofColor color;
    if(m_colors.find(colorName)!= m_colors.end()){
        color = m_colors[colorName];
    }
    
    return color;
}









