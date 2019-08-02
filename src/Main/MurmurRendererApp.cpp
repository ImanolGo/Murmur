/*
 *  MurmurRendererApp.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 19/06/15.
 *
 */

#include "AppManager.h"

#include "MurmurRendererApp.h"

//--------------------------------------------------------------
void MurmurRendererApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void MurmurRendererApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void MurmurRendererApp::draw(){
    AppManager::getInstance().draw();
}

void MurmurRendererApp::drawScreen2(ofEventArgs & args){
    AppManager::getInstance().draw2();
}

void MurmurRendererApp::drawScreen3(ofEventArgs & args){
    AppManager::getInstance().draw3();
}

void MurmurRendererApp::exit()
{
    ofLogNotice() <<"MurmurRenderer::Exit";
    ofExit();

}

void MurmurRendererApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);
    
    
}


