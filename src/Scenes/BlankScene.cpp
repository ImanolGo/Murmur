/*
 *  BlankScene.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 25/06/15.
 *
 */


#include "BlankScene.h"

BlankScene::BlankScene(): ofxScene("BlankScene"), m_initialized(false)
{
    
}

BlankScene::~BlankScene()
{
    //Intentionally left empty
}


void BlankScene::setup() {
    ofLogNotice("BlankScene::setup");
}

void BlankScene::update() {
    
}

void BlankScene::draw() {
    ofBackground(0,0,0);
}

void BlankScene::willFadeIn() {
     ofLogNotice("BlankScene::willFadeIn");
}

void BlankScene::willDraw() {
    ofLogNotice("BlankScene::willDraw");
}

void BlankScene::willFadeOut() {
    ofLogNotice("BlankScene::willFadeOut");
}

void BlankScene::willExit() {
    ofLogNotice("BlankScene::willExit");
}
