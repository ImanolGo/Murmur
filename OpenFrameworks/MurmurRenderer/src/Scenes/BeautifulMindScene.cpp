/*
 *  BeautifulMindScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 02/07/15.
 *
 */


#include "AppManager.h"
#include "BeautifulMindScene.h"



BeautifulMindScene::BeautifulMindScene(): ofxScene("BeautifulMindScene"), m_initialized(false)
{
    //! Intentionally left empty
}


BeautifulMindScene::~BeautifulMindScene()
{
    //! Intentionally left empty
}


void BeautifulMindScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    m_initialized = true;
    
    this->setupImages();
    this->setupFbos();
    this->setupVideo();
    this->setupShader();

    ofLogNotice("BeautifulMindScene::setup");
    
}

void BeautifulMindScene::setupFbos()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    m_fboVideo.allocate(windowsSettings.getWidth(), windowsSettings.getHeight(),GL_RGBA);
    m_fboVideo.begin(); ofClear(0, 0, 0);  ; m_fboVideo.end();

	m_maskFbo.allocate(windowsSettings.getWidth(), windowsSettings.getHeight(), GL_RGB);
	m_maskFbo.begin(); ofClear(0, 0, 0);  m_maskFbo.end();

	ofLogNotice() << "BeautifulMindScene::setupFbos -> w = " << m_maskFbo.getWidth() << ", h = " << m_maskFbo.getHeight();

	ofLogNotice() << "BeautifulMindScene::setupFbos -> w = " << m_fboVideo.getWidth() << ", h = " << m_fboVideo.getHeight();
    
}

void BeautifulMindScene::setupImages()
{
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    
    string resourceName = "frame_beautifulmind";
    ofPoint position = ofPoint(0,0,0);
    
    ofPtr<ImageVisual> image = ofPtr<ImageVisual>(new ImageVisual(position,resourceName));
     image->setWidth(windowsSettings.getWidth()); image->setHeight(windowsSettings.getHeight());
    m_images[resourceName] = image;
    
    resourceName = "calibration_image";
    image = ofPtr<ImageVisual>(new ImageVisual(position,resourceName));
    image->setHeight(windowsSettings.getHeight()); image->setWidth(windowsSettings.getWidth());
    m_images[resourceName] = image;

	ofLogNotice() << "BeautifulMindScene::setupImages -> w = " << m_images[resourceName]->getWidth() << ", h = " << m_images[resourceName]->getHeight();
    
}


void BeautifulMindScene::setupShader()
{
   
	string path = "shaders/shadersGL2/BlackMask";
	if (ofIsGLProgrammableRenderer()) {
		path = "shaders/shadersGL3/BlackMask";
	}

	m_maskShader.load(path);
	ofLogNotice() << "BeautifulMindScene::setupShader -> " << path;

    this->setupMask();
}


void BeautifulMindScene::setupMask()
{

	string resourceName = "frame_beautifulmind";
	auto texture = AppManager::getInstance().getResourceManager().getTexture(resourceName);

    // creating masker texture
    m_maskFbo.begin();
        ofClear(0);
        //m_images["frame_beautifulmind"]->draw();
		texture->draw(0, 0, texture->getWidth(), texture->getHeight());
    // draw masker here in gray scale
	m_maskFbo.end();

	ofLogNotice() << "BeautifulMindScene::setupMask -> w = " << m_maskFbo.getWidth() << ", h = " << m_maskFbo.getHeight();
}

void BeautifulMindScene::setupVideo()
{
    string videoFileName = "videos/BeautifulMind.mov";
	m_video.load(videoFileName);
    m_video.setLoopState(OF_LOOP_NORMAL);

	ofLogNotice() << "BeautifulMindScene::setupVideo -> w = " << m_video.getWidth() << ", h = " << m_video.getHeight();
}



void BeautifulMindScene::update()
{
    this->updateVideo();
    //this->updateMask();
}

void BeautifulMindScene::updateVideo()
{
    m_video.update();
    
    //auto area = getDrawingArea();
    //m_video.setHeight(area.height); m_video.setWidth(area.width);
    //ofVec2f pos(area.x,area.y);
    //m_video.setPosition(pos);

	if (m_video.isFrameNew())
	{
		ofEnableAlphaBlending();
		m_fboVideo.begin();
			ofClear(0);
			m_maskShader.begin();
			m_maskShader.setUniformTexture("imageMask", m_maskFbo.getTexture(), 1);
			m_video.draw(0,0, m_maskFbo.getWidth(), m_maskFbo.getHeight());
			m_maskShader.end();

			//m_maskFbo.draw(0,0);
		m_fboVideo.end();
	}
    
}


void BeautifulMindScene::updateMask()
{
    string resourceName = "frame_beautifulmind";
    
    auto area = getDrawingArea();
    m_images[resourceName]->setHeight(area.height); m_images[resourceName]->setWidth(area.width);
    ofVec2f pos(area.x,area.y);
    
    m_images[resourceName]->setPosition(pos);
    
    this->setupMask();
}



void BeautifulMindScene::draw() {
   
   ofClear(0);
    
    bool calibration = AppManager::getInstance().getBeautifulMindManager().isCalibrationOn();
    
    if(calibration){
        this->drawCalibration();
    }
    else{
        this->drawScene();
    }
    
   
}

void BeautifulMindScene::drawScene()
{
	ofEnableAlphaBlending();
    this->drawVideo();
}

void BeautifulMindScene::drawCalibration()
{
    string resourceName = "calibration_image";
    
    auto area = getDrawingArea();
    m_images[resourceName]->setHeight(area.height); m_images[resourceName]->setWidth(area.width);
    ofVec2f pos(area.x,area.y);
    
    m_images[resourceName]->setPosition(pos);
    
    m_images[resourceName]->draw();

}

void BeautifulMindScene::drawVideo() {
    
	auto area = getDrawingArea();
    m_fboVideo.draw(area);
}

ofRectangle BeautifulMindScene::getDrawingArea()
{
    auto rect = AppManager::getInstance().getBeautifulMindManager().getRectangleSpace();
    auto windowsSettings = AppManager::getInstance().getSceneManager().getWindowSettings(this);
    
    rect.x *= windowsSettings.getWidth();
    rect.width *= windowsSettings.getWidth();
    rect.y *= windowsSettings.getHeight();
    rect.height *= windowsSettings.getHeight();
    
    return rect;
}

void BeautifulMindScene::willFadeIn() {
     ofLogNotice("BeautifulMindScene::willFadeIn");
    
     m_video.stop();
     m_video.setFrame(0);
     m_video.play();
    
}

void BeautifulMindScene::willDraw() {
    ofLogNotice("BeautifulMindScene::willDraw");
}

void BeautifulMindScene::willFadeOut() {
    ofLogNotice("BeautifulMindScene::willFadeOut");
}

void BeautifulMindScene::willExit() {
    ofLogNotice("BeautifulMindScene::willExit");
    
    m_video.stop();
}
