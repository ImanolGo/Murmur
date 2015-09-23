/*
 *  BirdsManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 05/08/15.
 *
 */


#include "AppManager.h"
#include "BirdsManager.h"


BirdsManager::BirdsManager()
{
    //Intentionaly left empty
}


BirdsManager::~BirdsManager()
{
    ofLogNotice() <<"BirdsManager::Destructor" ;
}


void BirdsManager::setup()
{

    Manager::setup();
    
    ofLogNotice() <<"BirdsManager::initialized" ;
    
    this->setupSyphon();
    this->setupShader();
    this->setupEffects();
}


void BirdsManager::setupSyphon()
{
    int windowIndex = 1;
    auto windowsSettings = AppManager::getInstance().getSettingsManager().getWindowsSettings(windowIndex);
    
    m_syphonVisual.setWidth(windowsSettings.width); m_syphonVisual.setHeight(windowsSettings.height);
    
    //m_syphonVisual.setup("","Simple Server");
    //m_syphonVisual.setup("Main Camera","Unity");
    m_syphonVisual.setup("Main Camera","birds");
}

void BirdsManager::setupShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shader.load("shaders/shadersGL3/InverterShader");
    }
    else{
        m_shader.load("shaders/shadersGL2/InverterShader");
    }
    
    ofLogNotice() <<"ContourManager::setupShader";
}

void BirdsManager::setupEffects()
{
    m_swarm =  ofPtr<BirdsSwarmVisual>(new BirdsSwarmVisual());
    m_moveEffect = ofPtr<MoveVisual>(new MoveVisual(m_swarm));
    m_scaleEffect = ofPtr<ScaleVisual>(new ScaleVisual(m_swarm));
}



void BirdsManager::update()
{
    if(!m_moveEffect->isFinished()){
         //ofLogNotice() <<"ContourManager::update -> " << m_swarm->getPosition().y ;
        AppManager::getInstance().getGuiManager().setBirdsPosition(m_swarm->getPosition());
    }
    
    if(!m_scaleEffect->isFinished()){
        AppManager::getInstance().getGuiManager().setBirdsSize(m_swarm->getScale());
    }
}

void BirdsManager::draw()
{
    //m_shader.begin();
      //  m_syphonVisual.draw();
    //m_shader.end();
    
    m_syphonVisual.draw();
}


void BirdsManager::onChangePosition(ofVec3f& target)
{
    float scale = 50;
    m_swarm->setPosition(target);
    
    ofxOscMessage m;
    m.setAddress("/MurmurBirds/position/x");
    m.addFloatArg(target.x*scale);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);
    
    scale = 30;
    m.clear();
    m.setAddress("/MurmurBirds/position/y");
    m.addFloatArg(target.y*scale);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);
    
    scale = 30;
    m.clear();
    m.setAddress("/MurmurBirds/position/z");
    m.addFloatArg(target.z*scale);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);
}

void BirdsManager::onChangeSize(float& value)
{
    m_swarm->m_birdSize = value;
    
    ofxOscMessage m;
    m.setAddress("/MurmurBirds/birdSize");
    m.addFloatArg(value);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);

}

void BirdsManager::onChangeSpeed(float& value)
{
    m_swarm->m_speed = value;
    
    ofxOscMessage m;
    m.setAddress("/MurmurBirds/speed");
    m.addFloatArg(value);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);
    
}

/*void BirdsManager::onChangeSwarmSize(float& value)
{
    m_birdsSwarmSize = value;
    
    ofxOscMessage m;
    m.setAddress("/MurmurBirds/swarmSize");
    m.addFloatArg(m_birdsSwarmSize);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);
}*/

void BirdsManager::onChangeSwarmSize(ofVec3f& size)
 {
     
     m_swarm->setScale(size);
     
     ofxOscMessage m;
     m.setAddress("/MurmurBirds/swarmSize/width");
     m.addFloatArg(size.x);
     AppManager::getInstance().getOscManager().sendMessageToUnity(m);
     
     m.clear();
     m.setAddress("/MurmurBirds/swarmSize/height");
     m.addFloatArg(size.y);
     AppManager::getInstance().getOscManager().sendMessageToUnity(m);
     
     m.clear();
     m.setAddress("/MurmurBirds/swarmSize/depth");
     m.addFloatArg(size.z);
     AppManager::getInstance().getOscManager().sendMessageToUnity(m);
}

void BirdsManager::onChangeSwarmNumber(int& value)
{
    m_swarm->m_number = value;
    
    ofxOscMessage m;
    m.setAddress("/MurmurBirds/swarmNumber");
    m.addFloatArg(value);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);
}

void BirdsManager::onChangeSwarmRandomness(float& value)
{
    m_swarm->m_randomness = value;
    
    ofxOscMessage m;
    m.setAddress("/MurmurBirds/swarmRandomness");
    m.addFloatArg(value);
    AppManager::getInstance().getOscManager().sendMessageToUnity(m);
}


void BirdsManager::addMoveEffect(const ofVec3f& targetPosition, double duration, double startTime)
{
    m_moveEffect->stop();
    m_moveEffect->setParameters(targetPosition, duration);
    m_moveEffect->start(startTime);
    
    AppManager::getInstance().getVisualEffectsManager().addVisualEffect(m_moveEffect);
}

void BirdsManager::addScaleEffect(const ofVec3f& targetScale, double duration, double startTime)
{
    m_scaleEffect->stop();
    m_scaleEffect->setParameters(targetScale, duration);
    m_scaleEffect->start(startTime);
    
    AppManager::getInstance().getVisualEffectsManager().addVisualEffect(m_scaleEffect);
    
}

void BirdsManager::stopEffects()
{
    m_scaleEffect->stop();    
}






