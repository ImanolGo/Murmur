/*
 *  AppManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#include "ofMain.h"


#include "AppManager.h"
#include "WindowSettingsManager.h"

AppManager& AppManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static AppManager    m_instance;
    return m_instance;

}

AppManager::AppManager(): Manager(), m_debugMode(true), m_initialized(false)
{
    //Intentioanlly left empty
}


AppManager::~AppManager()
{
    ofLogNotice() <<"AppManager::Destructor";
}


void AppManager::setup()
{
	if(m_initialized)
		return;
    
	Manager::setup();
    
     this->setupOF();
     this->setupManagers();
//
//
//    setDebugMode(m_debugMode);
    
    ofLogNotice() << "AppManager::initialized";
    m_initialized = true;

    
    ofSetBackgroundColor(0);
    
    //ofSetVerticalSync(false);
}

void AppManager::setupOF()
{
    ofLogNotice() << "AppManager::setupOF";
    
   // ofSetVerticalSync(true);
    ofSetEscapeQuitsApp(false);
    ofEnableAlphaBlending();
    //ofEnableSmoothing();
}


void AppManager::setupManagers()
{
    ofLogNotice() << "AppManager::setupManagers";
    
    //m_viewManager.setup();
    m_visualEffectsManager.setup();
    m_settingsManager.setup();
    m_resourceManager.setup();
    m_layoutManager.setup();
    m_contourManager.setup();
    m_handsManager.setup();
    m_floorManager.setup();
    m_beautifulMindManager.setup();
    m_oscManager.setup();
    m_udpManager.setup();
    m_sceneManager.setup();
    m_audioManager.setup();
    m_keyboardManager.setup();
    m_birdsManager.setup();
    //m_previewManager.setup();
    m_maskManager.setup();
    
    m_guiManager.setup();    

}
void AppManager::update()
{
    if(!m_initialized){
        return;
    }
    
    ofEnableAlphaBlending();

	m_audioManager.update();
	m_oscManager.update();

	m_udpManager.update();
	m_visualEffectsManager.update();
	m_contourManager.update();
	m_birdsManager.update();
	m_sceneManager.update();
	m_handsManager.update();

	m_layoutManager.update();
    
    m_guiManager.update();
}



void AppManager::draw()
{

	if (!m_initialized) {
		return;
	}

	ofEnableAlphaBlending();
	ofClear(0);
	ofBackground(40);
	this->draw1();
	this->draw2();
	this->draw3();

}

void AppManager::draw1()
{
	auto settings = WindowSettingsManager::getInstance().getWindowsSettings(0);

	ofPushStyle();
		ofTranslate(settings.getPosition());
		m_layoutManager.draw();
		m_guiManager.draw();
	ofPopStyle();
}

void AppManager::draw2()
{

	auto settings = WindowSettingsManager::getInstance().getWindowsSettings(1);

	ofPushStyle();
		ofTranslate(settings.getPosition());
		m_layoutManager.drawFront();
	ofPopStyle();
}

void AppManager::draw3()
{
	auto settings = WindowSettingsManager::getInstance().getWindowsSettings(2);

	ofPushStyle();
	ofTranslate(settings.getPosition());
	m_layoutManager.drawTop();
	ofPopStyle();

}


void AppManager::toggleDebugMode()
{
    m_debugMode = !m_debugMode;
    setDebugMode(m_debugMode);
    //m_viewManager.showDebugMode(m_debugMode);
}


void AppManager::setDebugMode(bool showDebug)
{
    m_debugMode = showDebug;
    
    ofLogNotice()<<"AppManager::setDebugMode-> " << m_debugMode;
    
    if(m_debugMode){
        //ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else{
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    
   // m_guiManager.showGui(m_debugMode);
}

void AppManager::setFullScreen(bool& value)
{
}



