/*
 *  GuiManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "LayoutManager.h"

#include "GuiManager.h"
#include "GuiTheme.h"

const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "MurmurRenderer";
const int GuiManager::GUI_WIDTH = 250;


GuiManager::GuiManager(): Manager(), m_showGui(true), m_width(0.0), m_height(0.0)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"GuiManager::initialized";
    
    Manager::setup();
    
    this->setupGuiParameters();
    this->setupScenesGui();
    this->setupBirdsGui();
    this->setupContourGui();
    this->setupHandsGui();
    this->setupFloorGui();
    this->setupLayoutGui();
    this->setupBeautifulMindGui();
    this->setupAudioGui();
    this->setupTopGui();
    this->loadGuiValues();
    
}

void GuiManager::setupGuiParameters()
{
    //create an actual ImGui context before setting up the addon
    ImGui::CreateContext();
    
    //specify a font to use
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(&ofToDataPath("fonts/roboto/Roboto-Regular.ttf")[0], 16.f);
    
    //ImGui::GetIO().FontGlobalScale = 1.0 / 2.0;
    
    //finally setup the addon ofxImGui::Gui setup;
    m_gui.setup(new GuiTheme());
    ofxImGui::Settings().windowPos  = ofVec2f(0,0);
    ofxImGui::Settings().windowSize = ofVec2f(GUI_WIDTH,ofGetHeight());
    
    m_width = 0;
    m_height = 0;
    
    auto appManager = &AppManager::getInstance();
    
}

void GuiManager::setupScenesGui()
{
    
    auto sceneManager = &AppManager::getInstance().getSceneManager();
    
    m_sceneNames.clear();
    m_parametersScenes.setName("Scenes");
    m_sceneMode.set("Scene", 0);
    m_sceneMode.addListener(sceneManager, &SceneManager::changeSceneIndex);
    m_parametersScenes.add(m_sceneMode);
    
    for(int i=0; i< sceneManager->getNumberScenes(); i++){
        m_sceneNames.push_back(sceneManager->getSceneName(i));
    }
    
    m_sceneTransitionTime.set("TransitionTime", 0.5, 0.0, 30);
    m_sceneTransitionTime.addListener(sceneManager, &SceneManager::onTransitionTimeChange);
    m_parameters.add(m_sceneTransitionTime);
    
    m_sceneOpacity.set("Opacity", 0.0, 0.0, 1.0);
    m_sceneOpacity.addListener(this, &GuiManager::onSetSceneOpacity);
    m_parameters.add(m_sceneOpacity);
    
    m_sceneOpacityFront.set("OpacityFront", 0.0, 0.0, 1.0);
    m_sceneOpacityFront.addListener(sceneManager, &SceneManager::onFrontOpacityChange);
    m_parameters.add(m_sceneOpacityFront);
    
    m_sceneOpacityTop.set("OpacityTop", 0.0, 0.0, 1.0);
    m_sceneOpacityTop.addListener(sceneManager, &SceneManager::onTopOpacityChange);
    m_parameters.add(m_sceneOpacityTop);
}


void GuiManager::setupBirdsGui()
{
    auto birdsManager = &AppManager::getInstance().getBirdsManager();
    
    m_parametersBirds.setName("Birds");
    
    m_birdsPosition.set("Position", ofVec3f(0,0,0) , ofVec3f(-1.0,-1.0,-1.0) , ofVec3f(1.0,1.0,1.0));
    m_birdsPosition.addListener(birdsManager, &BirdsManager::onChangePosition);
    m_parameters.add(m_birdsPosition);

    m_birdsSize.set("Size", 1.0, 0.0, 5.0);
    m_birdsSize.addListener(birdsManager, &BirdsManager::onChangeSize);
    m_parameters.add(m_birdsSize);
    
    m_birdsSwarmSize.set("SwarmSize", 4.0, 0.0, 30.0);
    m_birdsSwarmSize.addListener(this, &GuiManager::onSetBirdsSwarmSize);
    m_parameters.add(m_birdsSwarmSize);
    
    m_birdsSwarmSizeVector.set("SwarmSize",  ofVec3f(4,4,4) , ofVec3f(0.0,0.0,0.0) , ofVec3f(30,30,30));
    m_birdsSwarmSizeVector.addListener(birdsManager, &BirdsManager::onChangeSwarmSize);
    m_parameters.add(m_birdsSwarmSizeVector);
    
    m_birdsNumber.set("BirdsNumber", 250, 0, 1000);
    m_birdsNumber.addListener(birdsManager, &BirdsManager::onChangeSwarmNumber);
    m_parameters.add(m_birdsNumber);
    
    m_birdsSpeed.set("Speed", 10, 0, 20);
    m_birdsSpeed.addListener(birdsManager, &BirdsManager::onChangeSpeed);
    m_parameters.add(m_birdsSpeed);
    
    m_birdsRandomness.set("Randomness", 0, 0, 50);
    m_birdsRandomness.addListener(birdsManager, &BirdsManager::onChangeSwarmRandomness);
    m_parameters.add(m_birdsRandomness);
    
}

void GuiManager::setupAudioGui()
{
    auto audioManager = &AppManager::getInstance().getAudioManager();
    m_parametersAudio.setName("Audio");
    
    m_audioOn.set("AudioOn", false);
    m_audioOn.addListener(audioManager, &AudioManager::onChangeAudioOn);
    m_parameters.add(m_audioOn);
    
    m_audioVolume.set("Volume", 0.5, 0.0, 1.0);
    m_audioVolume.addListener(audioManager, &AudioManager::onChangeVolume);
    m_parameters.add(m_audioVolume);
    
}

void GuiManager::setupTopGui()
{
    auto oscManager = &AppManager::getInstance().getOscManager();
    m_parametersTop.setName("FloorTracking");
    
    m_paperThrowerSpeed.set("PaperThrowerSpeed", 0, 0, 100);
    m_paperThrowerSpeed.addListener(oscManager, &OscManager::onChangePaperThrowerSpeed);
    m_parameters.add(m_paperThrowerSpeed);
    
}

void GuiManager::setupContourGui()
{
    
    auto contourManager = &AppManager::getInstance().getContourManager();
    m_parametersContour.setName("Contour");
    
    m_contourThickness.set("Thickness", 1.0, 0.0, 4.0);
    m_contourThickness.addListener(contourManager, &ContourManager::setContourThickness);
    m_parameters.add(m_contourThickness);
    
    
    m_contourOffset.set("Offset", ofVec2f(0.0,0.0) , ofVec2f(-0.7,-0.7) , ofVec2f(0.7,0.7));
    m_contourOffset.addListener(contourManager, &ContourManager::setOffset);
    m_parameters.add(m_contourOffset);
    
    m_contourScale.set("Scale", ofVec2f(1.0,1.0) , ofVec2f(-2.0,-2.0) , ofVec2f(2.0,2.0) );
    m_contourScale.addListener(contourManager, &ContourManager::setScale);
    m_parameters.add(m_contourScale);
    
}

void GuiManager::setupHandsGui()
{
    
    auto handsManager = &AppManager::getInstance().getHandsManager();
    m_parametersHands.setName("Hands");
    
    m_handsOn.set("On", false);
    m_handsOn.addListener(handsManager, &HandsManager::setHandsOn);
    m_parameters.add(m_handsOn);
    
    m_handsOffset.set("Offset", ofVec2f(0.0,0.0) , ofVec2f(-0.5,-0.5) , ofVec2f(0.5,0.5) );
    m_handsOffset.addListener(handsManager, &HandsManager::setOffset);
    m_parameters.add(m_handsOffset);
    
    m_handsScale.set("Scale", ofVec2f(1.0,1.0) , ofVec2f(-1.5,-1.5) , ofVec2f(1.5,1.5) );
    m_handsScale.addListener(handsManager, &HandsManager::setScale);
    m_parameters.add(m_handsScale);
    
    m_handsSize.set("Size", 1.0, 0.0, 5.0);
    m_handsSize.addListener(handsManager, &HandsManager::setSize);
    m_parameters.add(m_handsSize);

    m_handsFadeTime.set("FadeTime", 2.0, 0.0, 15.0);
    m_handsFadeTime.addListener(handsManager, &HandsManager::setFadeTime);
    m_parameters.add(m_handsFadeTime);
    
}

void GuiManager::setupFloorGui()
{
    
    auto floorManager = &AppManager::getInstance().getFloorManager();
    m_parametersFloor.setName("Floor");
    
    m_floorOffset.set("Offset", ofVec2f(0.0,0.0) , ofVec2f(-0.5,-0.5) , ofVec2f(0.5,0.5) );
    m_floorOffset.addListener(floorManager, &FloorManager::setOffset);
    m_parameters.add(m_floorOffset);
    
    m_floorScale.set("Scale", ofVec2f(1.0,1.0) , ofVec2f(-2.5,-2.5) , ofVec2f(2.5,2.5) );
    m_floorScale.addListener(floorManager, &FloorManager::setScale);
    m_parameters.add(m_floorScale);
    
}

void GuiManager::setupBeautifulMindGui()
{
    
    auto beautifulMindManager = &AppManager::getInstance().getBeautifulMindManager();
    m_parametersBeautifulMind.setName("BeautifulMind");
    
    m_beautifulMindOffset.set("Offset", ofVec2f(0.0,0.0) , ofVec2f(-1.0,-1.0) , ofVec2f(1.0,1.0) );
    m_beautifulMindOffset.addListener(beautifulMindManager, &BeautifulMindManager::setOffset);
    m_parameters.add(m_beautifulMindOffset);
    
    m_beautifulMindScale.set("Scale", ofVec2f(1.0,1.0) , ofVec2f(0.0,0.0) , ofVec2f(1.0,1.0) );
    m_beautifulMindScale.addListener(beautifulMindManager, &BeautifulMindManager::setScale);
    m_parameters.add(m_beautifulMindScale);
    
    m_beautifulMindFloorOn.set("FloorOn", false);
    m_beautifulMindFloorOn.addListener(beautifulMindManager, &BeautifulMindManager::setFloorOn);
    m_parameters.add(m_beautifulMindFloorOn);
    
    m_beautifulMindFloorSpeed.set("Speed", 1.0, 0.0, 5.0);
    m_beautifulMindFloorSpeed.addListener(beautifulMindManager, &BeautifulMindManager::setFloorSpeed);
    m_parameters.add(m_beautifulMindFloorSpeed);
    
    m_beautifulMindCalibrationOn.set("CalibrationOn", false);
    m_beautifulMindCalibrationOn.addListener(beautifulMindManager, &BeautifulMindManager::setCalibrationOn);
    m_parameters.add(m_beautifulMindCalibrationOn);
}

void GuiManager::setupLayoutGui()
{
    auto appManager = &AppManager::getInstance();
    auto layoutManager = &AppManager::getInstance().getLayoutManager();
    
    m_parametersLayout.setName("Layout");

    m_cropLeft.set("CropLeft", 0.0, 0.0, 500.0);
    m_cropLeft.addListener(layoutManager, &LayoutManager::onCropLeft);
    m_parameters.add(m_cropLeft);
    
    m_cropRight.set("CropRight", 0.0, 0.0, 500.0);
    m_cropRight.addListener(layoutManager, &LayoutManager::onCropRight);
    m_parameters.add(m_cropRight);
    
    m_cropTop.set("CropTop", 0.0, 0.0, 500.0);
    m_cropTop.addListener(layoutManager, &LayoutManager::onCropTop);
    m_parameters.add(m_cropTop);
    
    m_cropBottom.set("CropBottom", 0.0, 0.0, 500.0);
    m_cropBottom.addListener(layoutManager, &LayoutManager::onCropBottom);
    m_parameters.add(m_cropBottom);
    
}

void GuiManager::update()
{
    //m_gui.update();
    m_gui.setTheme(new GuiTheme());
    
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    this->drawRectangle();
    this->drawGui();
}


void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
    
    m_gui.begin();
    
    auto mainSettings  = ofxImGui::Settings();
    //ofxImGui::Settings().windowPos  = ofVec2f(-LayoutManager::MARGIN,-LayoutManager::MARGIN);
    // ofxImGui::Settings().windowSize = ofVec2f(GUI_WIDTH,ofGetHeight());
    if (ofxImGui::BeginWindow(GUI_SETTINGS_NAME, mainSettings, false))
    {
        
        ImGui::Text("%.1f FPS (%.3f ms/frame)", ofGetFrameRate(), 1000.0f / ImGui::GetIO().Framerate);
        
        int oscPort = AppManager::getInstance().getSettingsManager().getOscPortReceive();
        
        if (ImGui::Button("Save Gui"))
        {
            this->saveGuiValues();
        }
        
        if (ImGui::Button("Load Gui"))
        {
            this->loadGuiValues();
        }
        
        
        if (ofxImGui::BeginTree(m_parametersScenes, mainSettings))
        {
            ofxImGui::AddParameter(m_sceneTransitionTime);
            ofxImGui::AddParameter(m_sceneOpacity);
            ofxImGui::AddParameter(m_sceneOpacityFront);
            ofxImGui::AddParameter(m_sceneOpacityTop);
            ofxImGui::AddCombo(m_sceneMode, m_sceneNames);
            ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree(m_parametersLayout, mainSettings))
        {
            ofxImGui::AddParameter(m_cropLeft);
            ofxImGui::AddParameter(m_cropRight);
            ofxImGui::AddParameter(m_cropTop);
            ofxImGui::AddParameter(m_cropBottom);
            ofxImGui::EndTree(mainSettings);
        }
        
        
    
        if (ofxImGui::BeginTree(m_parametersBirds, mainSettings))
        {
        
            ofxImGui::AddParameter(m_birdsPosition);
            ofxImGui::AddParameter(m_birdsSwarmSizeVector);
            ofxImGui::AddParameter(m_birdsSize);
            ofxImGui::AddParameter(m_birdsSwarmSize);
            ofxImGui::AddParameter(m_birdsNumber);
            ofxImGui::AddParameter(m_birdsSpeed);
            ofxImGui::AddParameter(m_birdsRandomness);
           
            ofxImGui::EndTree(mainSettings);
        }
        
        
        if (ofxImGui::BeginTree(m_parametersAudio, mainSettings))
        {
            ofxImGui::AddParameter(m_audioOn);
            ofxImGui::AddParameter(m_audioVolume);
            ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree(m_parametersTop, mainSettings))
        {
            //auto & group =
            
            if (ImGui::Button("PaperThrowerFire"))
            {
                AppManager::getInstance().getOscManager().onFirePaperThrower();
            }
            
            if(ImGui::Button("ResetBackground"))
            {
                AppManager::getInstance().getOscManager().onResetTopBackground();
            }
            
            ofxImGui::AddParameter(m_paperThrowerSpeed);
            ofxImGui::EndTree(mainSettings);
        }
    
        if (ofxImGui::BeginTree(m_parametersContour, mainSettings))
        {
            ofxImGui::AddParameter(m_contourThickness);
            ofxImGui::AddParameter(m_contourOffset);
            ofxImGui::AddParameter(m_contourScale);
            ofxImGui::EndTree(mainSettings);
        }
    
        if (ofxImGui::BeginTree(m_parametersHands, mainSettings))
        {
            ofxImGui::AddParameter(m_handsOffset);
            ofxImGui::AddParameter(m_handsScale);
            ofxImGui::AddParameter(m_handsOn);
            ofxImGui::AddParameter(m_handsFadeTime);
            ofxImGui::AddParameter(m_handsSize);
            ofxImGui::EndTree(mainSettings);
        }
    
        if (ofxImGui::BeginTree(m_parametersFloor, mainSettings))
        {
            ofxImGui::AddParameter(m_floorOffset);
            ofxImGui::AddParameter(m_floorScale);
            ofxImGui::EndTree(mainSettings);
        }
    
    
        if (ofxImGui::BeginTree(m_parametersBeautifulMind, mainSettings))
        {
            ofxImGui::AddParameter(m_beautifulMindOffset);
            ofxImGui::AddParameter(m_beautifulMindScale);
            ofxImGui::AddParameter(m_beautifulMindFloorSpeed);
            ofxImGui::AddParameter(m_beautifulMindFloorOn);
            ofxImGui::AddParameter(m_beautifulMindCalibrationOn);
            ofxImGui::EndTree(mainSettings);
        }
        
        
    }
    
    ofxImGui::EndWindow(mainSettings);
    m_gui.end();
    
    ofDisableAlphaBlending();
    
    this->updateSize(mainSettings);
    
    
}

void GuiManager::updateSize(const ofxImGui::Settings& settings)
{
    if(m_width!= settings.windowSize.x){
        m_width = settings.windowSize.x;
        AppManager::getInstance().getLayoutManager().windowResized(ofGetWidth(), ofGetHeight());
    }
    
    m_height = settings.windowSize.y;
    m_position = ofPoint(settings.windowPos.x, settings.windowPos.y);
}


void GuiManager::saveGuiValues(string path)
{
    ofLogNotice() <<"GuiManager::saveGuiValues-> saving values from: " << GUI_SETTINGS_FILE_NAME;
    
    ofXml xml;
    ofSerialize(xml, m_parameters);
    //xml.serialize(m_parameters);
    
    if(path.empty()){
        xml.save(GUI_SETTINGS_FILE_NAME);
    }
    else{
        xml.save(path);
    }
    
}

void GuiManager::loadGuiValues(string path)
{
    
    ofLogNotice() <<"GuiManager::loadGuiValues-> loading values from: " << GUI_SETTINGS_FILE_NAME;
    
    ofXml xml;
    if(path.empty()){
        xml.load(GUI_SETTINGS_FILE_NAME);
    }
    else{
        xml.load(path);
    }
    
    //xml.deserialize(m_parameters);
    ofDeserialize(xml, m_parameters);
}


 void GuiManager::onSetBirdsSwarmSize(float& value)
{
    m_birdsSwarmSize = value;
    ofVec3f size(value,value,value);
    m_birdsSwarmSizeVector.set(size);
    
}


void GuiManager::setScene(const string &sceneName)
{
    if(!m_parametersScenes.contains(sceneName)){
        return;
    }
    
    ofParameter<bool>& scene = (ofParameter<bool>&) m_parametersScenes.get(sceneName);
    scene.set(true);
}

void GuiManager::onSetSceneOpacity(float& value)
{
    m_sceneOpacityFront = value;
    m_sceneOpacityTop = value;
}

void GuiManager::drawRectangle()
{
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( 0, 0, this->getWidth(), ofGetHeight());
    ofPopStyle();
}

void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}


