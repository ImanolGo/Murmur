/*
 *  MurmurRendererApp.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 24/06/15.
 *
 */

#pragma once

#include "ofMain.h"
#include "MurmurRendererScreen.h"

class MurmurRendererApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
    
        void windowResized(int w, int h);
    
        void drawScreen2(ofEventArgs & args);
        //void keyPressed2(ofKeyEventArgs &e);
        void drawScreen3(ofEventArgs & args);
        //void keyPressed3(ofKeyEventArgs &e);

		shared_ptr<MurmurRendererScreen> window2;
		shared_ptr<MurmurRendererScreen> window3;
    
    private:
    
    bool isExit;
};

