/*
 *  ProjectorCalibrationSceneFloor.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 20/07/16.
 *
 */


#include "AppManager.h"
#include "ContourManager.h"
#include "SettingsManager.h"

#include "ProjectorCalibrationSceneFloor.h"

const float ProjectorCalibrationSceneFloor::LINE_THICKNESS = 5.0;


ProjectorCalibrationSceneFloor::ProjectorCalibrationSceneFloor(): ofxScene("ProjectorCalibrationScene"), m_initialized(false)
{
    
}

ProjectorCalibrationSceneFloor::~ProjectorCalibrationSceneFloor()
{
    //Intentionally left empty
}

void ProjectorCalibrationSceneFloor::setup()
{
    if (m_initialized) {
        return;
    }
    
    m_initialized = true;
    
    this->setupFbos();
    this->setupShaders();
    
    ofLogNotice("ProjectorCalibrationSceneFloor::setup");
    
}

void ProjectorCalibrationSceneFloor::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    m_fboTestPattern.allocate(windowsSettings.getWidth(), windowsSettings.getHeight());
    
    m_fboTestPattern.begin();
        ofClear(0);
        this->drawTestPattern();
    m_fboTestPattern.end();
}

void ProjectorCalibrationSceneFloor::setupShaders()
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

void ProjectorCalibrationSceneFloor::update()
{
    //Intenionally left empty
}


void ProjectorCalibrationSceneFloor::draw() {
    ofBackground(0,0,0);
    m_fboTestPattern.draw(0, 0);
    drawFansCircle();
}


void ProjectorCalibrationSceneFloor::drawTestPattern()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings((ofxScene *) this);
    float ratio = windowsSettings.getWidth() / windowsSettings.getHeight();
    float tileSize = windowsSettings.getWidth()/16;
    float rectSize = 3;
    
    
    ofPushStyle();
        ofSetCircleResolution(100);
        m_thickLineShader.begin();
        m_thickLineShader.setUniform1f("thickness", LINE_THICKNESS);
        ofSetColor(ofColor::white);
        ofSetLineWidth(LINE_THICKNESS);
            ofLine(windowsSettings.getWidth()*0.5, 0, windowsSettings.getWidth()*0.5, windowsSettings.getHeight());
            ofLine(0, windowsSettings.getHeight()*0.5, windowsSettings.getWidth(), windowsSettings.getHeight()*0.5);
    
            ofLine(LINE_THICKNESS*0.5, 0, LINE_THICKNESS*0.5, windowsSettings.getHeight());
            ofLine(0, LINE_THICKNESS*0.5, windowsSettings.getWidth(), LINE_THICKNESS*0.5);
            ofLine(windowsSettings.getWidth() - LINE_THICKNESS*0.5, 0, windowsSettings.getWidth() - LINE_THICKNESS*0.5, windowsSettings.getHeight());
            ofLine(0,  windowsSettings.getHeight() - LINE_THICKNESS*0.5, windowsSettings.getWidth(),  windowsSettings.getHeight() - LINE_THICKNESS*0.5);
    
        m_thickLineShader.end();
    
        ofSetColor(ofColor::white);
        ofSetLineWidth(1.0);
        for(int i = 0; i <= 16; i++)
        {
            ofLine(i*tileSize, 0, i*tileSize, windowsSettings.getHeight());
            ofLine(0, i*tileSize, windowsSettings.getWidth(), i*tileSize);
        
            for(int j = 0; j <= 16; j++)
            {
                float x = i*tileSize + tileSize*0.5 - rectSize*0.5;
                float y = j*tileSize + tileSize*0.5 - rectSize*0.5;
                ofRect(x, y, rectSize, rectSize);
            }
        
        }
    
    ofPopStyle();


}

void ProjectorCalibrationSceneFloor::drawFansCircle()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    
    ofPoint pos = AppManager::getInstance().getLayoutManager().getCircleOffset();
    
    ofPushMatrix();
    ofPushStyle();
        ofTranslate(pos.x, pos.y);
        ofSetCircleResolution(100);
        m_thickLineShader.begin();
        m_thickLineShader.setUniform1f("thickness", LINE_THICKNESS*0.5);
    
        ofSetColor(ofColor::yellow);
        ofNoFill();
            ofDrawCircle(windowsSettings.getWidth()*0.5, windowsSettings.getHeight()*0.5, windowsSettings.getHeight()*0.5);
    
        ofFill();
        for(int i = 0; i < 10 ; i++){
            float distance = windowsSettings.getWidth();
            float angle = i*2*PI/10;
            float x = windowsSettings.getWidth()*0.5 + distance * cos(angle);
            float y = windowsSettings.getHeight()*0.5 + distance * sin(angle);
            ofDrawLine(windowsSettings.getWidth()*0.5, windowsSettings.getHeight()*0.5, x, y);
        }
    
        m_thickLineShader.end();
    ofPopStyle();
    ofPopMatrix();

}

void ProjectorCalibrationSceneFloor::willFadeIn() {
    ofLogNotice("ProjectorCalibrationSceneFloor::willFadeIn");
}

void ProjectorCalibrationSceneFloor::willDraw() {
    ofLogNotice("ProjectorCalibrationSceneFloor::willDraw");
}

void ProjectorCalibrationSceneFloor::willFadeOut() {
    ofLogNotice("ProjectorCalibrationSceneFloor::willFadeOut");
}

void ProjectorCalibrationSceneFloor::willExit() {
    ofLogNotice("ProjectorCalibrationSceneFloor::willExit");
}
