/*
 *  SceneManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 25/06/15.
 *
 */

#include "ofMain.h"


#include "SceneManager.h"
#include "scenes.h"
#include "WindowSettingsManager.h"

#include "AppManager.h"

SceneManager::SceneManager(): Manager()
{
	//Intentionally left empty
}


SceneManager::~SceneManager()
{
    ofLogNotice() <<"SceneManager::Destructor";
}


void SceneManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();

    this->createSceneManagers();
    this->createScenes();
    //this->setupText();

    ofLogNotice() <<"SceneManager::initialized";

}

void SceneManager::createSceneManagers()
{
    m_sceneManagers[WindowIndex(FRONT)] = ofPtr<ofxSceneManager> (new ofxSceneManager());
    m_sceneManagers[WindowIndex(FRONT)]->setTransitionDissolve();
    //m_sceneManagers[WindowIndex(FRONT)]->setSceneDuration(3.5,3.5);
    
    m_sceneManagers[WindowIndex(TOP)] = ofPtr<ofxSceneManager> (new ofxSceneManager());
    m_sceneManagers[WindowIndex(TOP)]->setTransitionDissolve();
    //m_sceneManagers[WindowIndex(TOP)]->setSceneDuration(3.5,3.5);
}


void SceneManager::createScenes()
{
    this->createFrontScenes();
    this->createTopScenes();
    this->createSceneNames();
    
    m_sceneManagers[WindowIndex(FRONT)]->run();
    m_sceneManagers[WindowIndex(TOP)]->run();
}

void SceneManager::createSceneNames()
{
    vector<string> names = {"BlankScene", "SmokyHandsScene", "BattleOfSelfScene", "HandsWritingScene","BeautifulMindScene","BirdsAndPaperScene", "BirdsOpeningScene", "SonicBoomScene", "ProjectorCalibrationScene"};
    m_sceneNames = names;
}

void SceneManager::createFrontScenes()
{
    int i = 0;
    ofPtr<ofxScene> scene;
    
    //Create Blank Scene
    scene = ofPtr<ofxScene> (new BlankScene());
    this->createScene("BlankScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
    //Create Smoky Hands Scene
    scene = ofPtr<ofxScene> (new SmokyHandsScene());
    this->createScene("SmokyHandsScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
    //Create Battle of Self Scene
    scene = ofPtr<ofxScene> (new BattleOfSelfScene());
    this->createScene("BattleOfSelfScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
    //Create Hands Writing Scene
    scene = ofPtr<ofxScene> (new HandsWritingScene());
    this->createScene("HandsWritingScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
    //Create Beautiful Mind Scene
    scene = ofPtr<ofxScene> (new BeautifulMindScene());
    this->createScene("BeautifulMindScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
    //Create Birds And Paper Scene
    scene = ofPtr<ofxScene> (new BirdsAndPaperScene());
    this->createScene("BirdsAndPaperScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
    //Create Birds Opening Scene
    scene = ofPtr<ofxScene> (new BirdsOpeningScene());
    this->createScene("BirdsOpeningScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
   
    //Create Sonic Boom Scene
    scene = ofPtr<ofxScene> (new SonicBoomScene());
    this->createScene("SonicBoomScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
    //Create ProjectorCalibrationScene Scene
    scene = ofPtr<ofxScene> (new ProjectorCalibrationScene());
    this->createScene("ProjectorCalibrationScene", SceneIndex(i), scene, WindowIndex(FRONT));
    i++;
    
}

void SceneManager::createTopScenes()
{
    
    int i = 0;
    ofPtr<ofxScene> scene;
    
    //Create Blank Scene
    scene = ofPtr<ofxScene> (new BlankScene());
    this->createScene("BlankScene", SceneIndex(i), scene, WindowIndex(TOP));
    i++;
    
    scene = ofPtr<ofxScene> (new FluidFloorScene());
    this->createScene("FluidFloorScene", SceneIndex(i), scene, WindowIndex(TOP));
    i++;
    
    scene = ofPtr<ofxScene> (new KathakScene());
    this->createScene("KathakScene", SceneIndex(i), scene, WindowIndex(TOP));
    i++;
    
    scene = ofPtr<ofxScene> (new BeautifulMindSceneFloor());
    this->createScene("BeautifulMindScene", SceneIndex(i), scene, WindowIndex(TOP));
    i++;
    
    scene = ofPtr<ofxScene> (new BirdsAndPaperSceneFloor());
    this->createScene("BirdsAndPaperScene", SceneIndex(i), scene, WindowIndex(TOP));
    i++;
    
    
    scene = ofPtr<ofxScene> (new ProjectorCalibrationSceneFloor());
    this->createScene("ProjectorCalibrationScene", SceneIndex(i), scene, WindowIndex(TOP));
    i++;
    
    
}


void SceneManager::createScene(string name, SceneIndex i, ofPtr<ofxScene> ofx_scene, WindowIndex w)
{
    ofPtr<Scene> scene = ofPtr<Scene> (new Scene());
    
    scene->index = i;
    scene->name = name;
    scene->window_index = w;
    scene->scene = ofx_scene;
    
    m_scenes.push_back(scene);

    ofLogNotice() <<"SceneManager::createScene -> name =  " << name << ", scene index = " << i << ", window index = " << w ;
    
    this->addSceneToSceneManager(scene, w);
}


bool SceneManager::addSceneToSceneManager(ofPtr<Scene> scene, WindowIndex w)
{
    if(m_sceneManagers.find(w)!= m_sceneManagers.end()){
        m_sceneManagers[w]->addScene(scene->scene);
        ofLogNotice() <<"SceneManager::addSceneToSceneManager -> Added  "<< scene->name << " to Scene Manager " << w;
        return true;
    }
    
    
    ofLogNotice() <<"SceneManager::addSceneToSceneManager -> No Scene Manager attached to window " << w;
    return false;
}
  

void SceneManager::setupText()
{
    auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings(0);
    auto guiManager = AppManager::getInstance().getGuiManager();
    
    ofVec3f position;
    
    float width =  (windowSettings.getWidth() - 4*LayoutManager::MARGIN - guiManager.getWidth())*0.5 - LayoutManager::MARGIN;
    int fontSize = 12;
    float height = fontSize*3;
    
    
    string text = "SCENE";
    float width_offset = (windowSettings.getWidth() - 4*LayoutManager::MARGIN - guiManager.getWidth())*0.5;
    
    position.x = guiManager.getWidth() + 2.5*LayoutManager::MARGIN + LayoutManager::MARGIN  + width_offset;
    position.y = LayoutManager::MARGIN + windowSettings.getHeight()*0.5;
    
    ofPtr<TextVisual> textVisual = ofPtr<TextVisual>(new TextVisual(position, width, height));
    textVisual->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    textVisual->setColor(ofColor::white);
    
    AppManager::getInstance().getViewManager().addOverlay(textVisual);
    
    position.x -= LayoutManager::MARGIN*0.5;
    position.y -= LayoutManager::MARGIN*0.5;
    height = textVisual->getHeight() + LayoutManager::MARGIN;
    width = textVisual->getWidth() + LayoutManager::MARGIN;
    ofPtr<RectangleVisual> rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    ofColor color(60,60,60);
    rectangleVisual->setColor(color);
    
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);
    
    
    fontSize = 36;
    width =  (windowSettings.getWidth() - 4*LayoutManager::MARGIN - guiManager.getWidth())*0.5 - LayoutManager::MARGIN;
    height = fontSize*3;
    
    position.x = guiManager.getWidth() + 2.5*LayoutManager::MARGIN + LayoutManager::MARGIN  + width_offset;
    position.y = LayoutManager::MARGIN + rectangleVisual->getPosition().y + rectangleVisual->getHeight();
    
    m_sceneText =  ofPtr<TextVisual>(new TextVisual(position, width, height));
    m_sceneText->setText("", "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_sceneText->setColor(ofColor::white);
    AppManager::getInstance().getViewManager().addOverlay(m_sceneText);
    
    width = m_sceneText->getWidth() + LayoutManager::MARGIN;
    height = m_sceneText->getHeight() + LayoutManager::MARGIN;
    
    position.x -= LayoutManager::MARGIN*0.5;
    position.y =  0.5*LayoutManager::MARGIN + rectangleVisual->getPosition().y + rectangleVisual->getHeight();
    
    m_sceneRect = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    m_sceneRect->setColor(color);
    
    AppManager::getInstance().getViewManager().addOverlay(m_sceneRect,2);
}


void SceneManager::updateText(const string& text)
{
    
    m_sceneText->setText(text);
    
    float width = m_sceneText->getWidth() + LayoutManager::MARGIN;
    float height = m_sceneText->getHeight() + LayoutManager::MARGIN;
    m_sceneRect->setHeight(height);
    m_sceneRect->setWidth(width);
    
}



bool SceneManager::run(WindowIndex w)
{
    if(m_sceneManagers.find(w)!= m_sceneManagers.end()){
        m_sceneManagers[w]->run();
        ofLogNotice() <<"SceneManager::runSceneManager -> Running Scene Manager attached to window " << w;
        return true;
    }
    
    ofLogNotice() <<"SceneManager::runSceneManager -> No Scene Manager attached to window " << w;
    return false;
}

void SceneManager::update()
{
     for(auto sceneManager : m_sceneManagers) {
         sceneManager.second->update();
     }
}

void SceneManager::draw(WindowIndex w)
{
    ofEnableAlphaBlending();
    if(m_sceneManagers.find(w)!= m_sceneManagers.end()){
        //ofPushStyle();
    
            m_sceneManagers[w]->draw();
        //ofDisableAlphaBlending();
        //ofPopStyle();
        return;
    }
    
    
    
    ofLogVerbose() <<"SceneManager::draw -> No Scene Manager attached to window " << w;
}


bool SceneManager::changeScene(string sceneName)
{
    
    for(auto sceneManager : m_sceneManagers)
    {
        bool sceneFound = false;
        
        //ofLogNotice() <<"SceneManager::changeScene -> sceneManager "<< sceneManager.first ;
        
        for(auto scene : m_scenes)
        {
            if(scene->name == sceneName && sceneManager.first == scene->window_index ){
                sceneFound = true;
                ofLogNotice() <<"SceneManager::changeScene -> Changed screen "<< sceneManager.first  <<" to scene  " << sceneName;
                sceneManager.second->changeScene((int) scene->index);
                //this->updateText(sceneName);
            }
        }
        
        if(!sceneFound){
            ofLogNotice() <<"SceneManager::changeScene ->  Changed screen "<< sceneManager.first  <<" to Black Scene ";
            sceneManager.second->changeScene(0);
        }
       
    }
    
    return true;
}

void SceneManager::changeSceneIndex(int& sceneIndex)
{
    if(sceneIndex < 0 || sceneIndex >=m_sceneNames.size()){
        return;
    }
    
    ofLogNotice() <<"SceneManager::changeSceneIndex << scene index " << sceneIndex;
    
    this->changeScene(m_sceneNames[sceneIndex]);
}



void SceneManager::onTransitionTimeChange(float & value){
  
    for(auto scene : m_scenes) {
        scene->scene->setSceneDuration(value,value);
    }
}

ofGLFWWindowSettings SceneManager::getWindowSettings(ofxScene* scene)
{
    for(auto scene_ : m_scenes) {
        if(scene_->scene.get() == scene){
            return WindowSettingsManager::getInstance().getWindowsSettings((int) scene_->window_index);
        }
    }
    
    return WindowSettingsManager::getInstance().getWindowsSettings(0);
    
}

void SceneManager::onOpacityChange(float & value)
{
    float alpha = ofMap(value, 0, 1, 1, 0, true);
    
    for(auto sceneManager : m_sceneManagers) {
        sceneManager.second->setAlpha(alpha);
    }
}

void SceneManager::onOpacityChange(float & value, WindowIndex w)
{
    float alpha = ofMap(value, 0, 1, 1, 0, true);
    
    if(m_sceneManagers.find(w)!= m_sceneManagers.end()){
        m_sceneManagers[w]->setAlpha(alpha);
    }
}


void SceneManager::onFrontOpacityChange(float & value)
{
    float alpha = ofMap(value, 0, 1, 1, 0, true);
    
    WindowIndex w =  WindowIndex(FRONT);
    if(m_sceneManagers.find(w)!= m_sceneManagers.end()){
        m_sceneManagers[w]->setAlpha(alpha);
    }
}

void SceneManager::onTopOpacityChange(float & value)
{
    float alpha = ofMap(value, 0, 1, 1, 0, true);
    
    WindowIndex w =  WindowIndex(TOP);
    if(m_sceneManagers.find(w)!= m_sceneManagers.end()){
        m_sceneManagers[w]->setAlpha(alpha);
    }
}







