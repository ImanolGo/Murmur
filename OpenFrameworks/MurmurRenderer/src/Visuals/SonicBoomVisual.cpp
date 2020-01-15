/*
 *  SonicBoomVisual.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 31/07/15.
 *
 */

#include "SonicBoomVisual.h"
#include "WindowSettingsManager.h"
#include "AppManager.h"



SonicBoomParticle::SonicBoomParticle(const ofPoint& pos, int index): BasicVisual(pos,0,0), m_index(index), m_lifeTime(5), m_live(true), m_time(0), m_initSize(0.0)
{
    this->setup();
}


SonicBoomParticle::~SonicBoomParticle()
{
    //Intentionaly left empty
}


void SonicBoomParticle::setup()
{
    m_time = 0;
    m_initSize = 0 + ofNoise( ofGetElapsedTimef()/4)*20;
    //m_lifeTime = 1 + ofNoise( ofGetElapsedTimef()/2)*2;
    m_lifeTime = 1 + ofRandom(2);
    
    m_size = 450 + ofNoise( ofGetElapsedTimef()/2)*350 ;
    m_color = ofColor::white;
    
    m_image.setResource("sonic_ring");
    m_image.setCentred(true);
    
}


void SonicBoomParticle::update()
{
    double dt = ofGetLastFrameTime();
    
    m_time += ofGetLastFrameTime();
    
    m_width = ofMap(m_time, 0, m_lifeTime, m_initSize, m_size);
    float brightness = ofMap(m_time, 0, m_lifeTime, 255, 0, true);
    m_color.setBrightness(brightness);
    //m_color.a = brightness;
   
    //m_color.a = brightness;
    
   // m_color.a = 50;
    
//    m_image.setColor(m_color);
//    m_image.setWidth(m_width,true);
//    m_image.setPosition(m_position);
    if(m_time>=m_lifeTime){
        m_live = false;
         //ofLogNotice()<< "brightness = " << m_color;
    }
    
}

void SonicBoomParticle::draw()
{
    //ofPushStyle();
    ofEnableAlphaBlending();
    //ofSetColor(m_color);
    //ofDrawCircle(m_image.getPosition(), m_image.getWidth());
    m_image.draw();
    //ofDisableAlphaBlending();
    //ofPopStyle();
}



SonicBoomVisual::SonicBoomVisual(): m_elapsedTime(10000), m_newParticleTime(0.05), m_index(0)
{
    this->setup();
}


SonicBoomVisual::~SonicBoomVisual()
{
    //Intentionaly left empty
}


void SonicBoomVisual::setup()
{
    m_img1.load("images/general/postit0.png");
    m_img2.load("images/general/postit4.png");
    
    this->setupFbo();
    this->setupShader();
    this->setupParticles();
}

void SonicBoomVisual::setupFbo()
{
    auto windowsSettings = WindowSettingsManager::getInstance().getWindowsSettings(1);
    
    m_fbo.allocate(windowsSettings.getWidth(), windowsSettings.getHeight(), GL_RGBA, 4);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void SonicBoomVisual::setupShader()
{
    string path = "shaders/shadersGL2/PointTexture";
    if(ofIsGLProgrammableRenderer()){
        path = "shaders/shadersGL3/PointTexture";
    }
    
    m_shader.load(path);
}

void SonicBoomVisual::setupParticles()
{
    auto windowsSettings = WindowSettingsManager::getInstance().getWindowsSettings(1);
    
    int   num = 200;
    for(int i = 0; i<num; i++ )
    {
        float x = ofRandom(windowsSettings.getWidth());
        float y = ofRandom(windowsSettings.getHeight());
        ofVec3f p(x, y, 0.0);
        m_points.push_back(p);
        
        float size = ofRandom(100, 400);
        size = 0;
        m_sizes.push_back(ofVec3f(size));
        m_colors.push_back(ofColor(255));
    }
    
    // upload the data to the vbo
    int total = (int)m_points.size();
    m_vbo.setVertexData(&m_points[0], total, GL_STATIC_DRAW);
    m_vbo.setNormalData(&m_sizes[0], total, GL_STATIC_DRAW);
    m_vbo.setColorData(&m_colors[0], total, GL_DYNAMIC_DRAW);
    
    // load the texure
    ofDisableArbTex();
    ofLoadImage(m_texture, "images/hands/ring4.png");
    ofEnableArbTex();
}

void SonicBoomVisual::update()
{
    this->updateParticles();
    this->updateFbo();
}


void SonicBoomVisual::updateParticles()
{
    auto hands = AppManager::getInstance().getHandsManager().getHands();
    auto windowsSettings = WindowSettingsManager::getInstance().getWindowsSettings(1);
    
    for (auto hand : hands) {
        ofPoint pos = hand;
        pos.x *= windowsSettings.getWidth();
        pos.y *= windowsSettings.getHeight();
        this->addParticle(pos);
    }
    
    for(ParticlesVector::iterator it = m_particles.begin(); it != m_particles.end();)
    {
        (*it)->update();
        m_colors[(*it)->getIndex()] = (*it)->getColor();
        m_points[(*it)->getIndex()] = (*it)->getPosition();
        m_sizes[(*it)->getIndex()] = ofVec3f((*it)->getWidth());
        
        if(!(*it)->isAlive()){
            m_sizes[(*it)->getIndex()] =  ofVec3f(0);
            it = m_particles.erase(it);
            
        }
        else{
            ++it;
        }
    }
    
    m_vbo.setVertexData(&m_points[0], m_points.size(), GL_STATIC_DRAW);
    m_vbo.setNormalData(&m_sizes[0], m_sizes.size(), GL_STATIC_DRAW);
    m_vbo.setColorData(&m_colors[0], m_colors.size(), GL_DYNAMIC_DRAW);
    
    
}

void SonicBoomVisual::updateFbo()
{
    ofEnableAlphaBlending();
    m_fbo.begin();
    ofClear(0,255);
    
    //    ofBackground(0);
    ////        ofDisableSmoothing();
    //    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //        for (auto particle: m_particles) {
    //            particle->draw();
    //        }
    //    ofDisableBlendMode();
    //    ofEnableSmoothing();
    
    //    m_img1.draw(0,0);
    //    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //    m_img2.draw(100,100);
    //    ofDisableBlendMode();
    
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    m_shader.begin();
    m_texture.bind();
    m_vbo.draw(GL_POINTS, 0, (int)m_points.size());
    m_texture.unbind();
    m_shader.end();
    ofDisablePointSprites();
    ofDisableBlendMode();
    m_fbo.end();
}

void SonicBoomVisual::draw()
{
    this->drawParticles();
}

void SonicBoomVisual::draw(int width, int height)
{
     m_fbo.draw(0,0, width, height);
}



void SonicBoomVisual::drawParticles()
{
    m_fbo.draw(0, 0);
}


void SonicBoomVisual::addParticle(const ofPoint &pos)
{
    m_elapsedTime += ofGetLastFrameTime();
    
    if (m_elapsedTime >= m_newParticleTime) {
        m_elapsedTime = 0.0;
        auto particle = ofPtr<SonicBoomParticle> (new SonicBoomParticle(pos, m_index));
        m_particles.push_back(particle);
        m_index = (m_index + 1)%m_points.size();
        //ofLogNotice() << "SonicBoomVisual::addParticle -> size =  " << m_particles.size();
    }
   
}

void SonicBoomVisual::clear()
{
    m_particles.clear();
    
    for(auto & size: m_sizes)
    {
        size = ofVec3f(0);
    }
    
    m_vbo.setNormalData(&m_sizes[0], m_sizes.size(), GL_STATIC_DRAW);
}
