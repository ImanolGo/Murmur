/*
 *  ProjectorCalibrationScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 20/07/16.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"
#include "SettingsManager.h"

#include "ProjectorCalibrationScene.h"

const float ProjectorCalibrationScene::LINE_THICKNESS = 5.0;


ProjectorCalibrationScene::ProjectorCalibrationScene(): ofxScene("ProjectorCalibrationScene"), m_initialized(false)
{
    
}

ProjectorCalibrationScene::~ProjectorCalibrationScene()
{
    //Intentionally left empty
}

void ProjectorCalibrationScene::setup()
{
    if (m_initialized) {
        return;
    }
    
    m_initialized = true;
    
    this->setupFbos();
    this->setupShaders();
    
    ofLogNotice("ProjectorCalibrationScene::setup");
    
}

void ProjectorCalibrationScene::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    ofLogNotice() << "ProjectorCalibrationScene::setupFbos -> x = "  << windowsSettings.getWidth() << ", y = " <<  windowsSettings.getHeight();
    
    m_fboTestPattern.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    
    m_fboTestPattern.begin();
        ofClear(0);
        this->drawTestPattern();
    m_fboTestPattern.end();
    
}


void ProjectorCalibrationScene::setupShaders()
{
    m_thickLineShader.setGeometryInputType(GL_LINES);
    m_thickLineShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    m_thickLineShader.setGeometryOutputCount(4);
    
    
    if(ofIsGLProgrammableRenderer()){
        m_thickLineShader.load("shaders/shadersGL3/ThickLineShaderVert.glsl", "shaders/shadersGL3/ThickLineShaderFrag.glsl", "shaders/shadersGL3/ThickLineShaderGeom.glsl");
    }
    else{
        m_thickLineShader.load("shaders/shadersGL2/ThickLineShaderVert.glsl", "shaders/shadersGL2/ThickLineShaderFrag.glsl", "shaders/shadersGL2/ThickLineShaderGeom.glsl");
    }
    
    printf("Maximum number of output vertices support is: %i\n", m_thickLineShader.getGeometryMaxOutputCount());
    
}

void ProjectorCalibrationScene::update()
{
    /// Intentionally left empty
}


void ProjectorCalibrationScene::drawTestPattern()
{
    //auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    float ratio = m_fboTestPattern.getWidth() / m_fboTestPattern.getHeight();
    float tileSize = m_fboTestPattern.getWidth()/16;
    float rectSize = 3;
    
    ofPushStyle();
            m_thickLineShader.begin();
            m_thickLineShader.setUniform1f("thickness", LINE_THICKNESS);
                ofSetColor(ofColor::white);
                ofSetLineWidth(LINE_THICKNESS);
                ofDrawLine(m_fboTestPattern.getWidth()*0.5, 0, m_fboTestPattern.getWidth()*0.5, m_fboTestPattern.getHeight());
                ofDrawLine(0, m_fboTestPattern.getHeight()*0.5, m_fboTestPattern.getWidth(), m_fboTestPattern.getHeight()*0.5);
    
                ofDrawLine(LINE_THICKNESS*0.5, 0, LINE_THICKNESS*0.5, m_fboTestPattern.getHeight());
                ofDrawLine(0, LINE_THICKNESS*0.5, m_fboTestPattern.getWidth(), LINE_THICKNESS*0.5);
                ofDrawLine(m_fboTestPattern.getWidth() - LINE_THICKNESS*0.5, 0, m_fboTestPattern.getWidth() - LINE_THICKNESS*0.5, m_fboTestPattern.getHeight());
                ofDrawLine(0,  m_fboTestPattern.getHeight() - LINE_THICKNESS*0.5, m_fboTestPattern.getWidth(),  m_fboTestPattern.getHeight() - LINE_THICKNESS*0.5);
    
            m_thickLineShader.end();
    
            ofSetColor(ofColor::white);
            ofSetLineWidth(1.0);
            for(int i = 0; i <= 16; i++)
            {
                ofDrawLine(i*tileSize, 0, i*tileSize, m_fboTestPattern.getHeight());
                ofDrawLine(0, i*tileSize, m_fboTestPattern.getWidth(), i*tileSize);
                
                for(int j = 0; j <= 16; j++){
                    
                    float x = i*tileSize + tileSize*0.5 - rectSize*0.5;
                    float y = j*tileSize + tileSize*0.5 - rectSize*0.5;
                    
                    ofDrawRectangle(x, y, rectSize, rectSize);
                }
                
            }
    
    ofPopStyle();

}

void ProjectorCalibrationScene::draw() {
    //ofBackground(0,0,0);
    ofClear(0);
    m_fboTestPattern.draw(0, 0);
    //this->drawTestPattern();
}



void ProjectorCalibrationScene::willFadeIn() {
    AppManager::getInstance().getLayoutManager().setIsMasked(false);
    ofLogNotice("ProjectorCalibrationScene::willFadeIn");
}

void ProjectorCalibrationScene::willDraw() {
    ofLogNotice("ProjectorCalibrationScene::willDraw");
}

void ProjectorCalibrationScene::willFadeOut() {
    AppManager::getInstance().getLayoutManager().setIsMasked(true);
    ofLogNotice("ProjectorCalibrationScene::willFadeOut");
}

void ProjectorCalibrationScene::willExit() {
    ofLogNotice("ProjectorCalibrationScene::willExit");
    
}
