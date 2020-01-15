#pragma once

#include "ofxNetwork.h"
#include "ofMain.h"

typedef union _data {
    float f;
    char  s[sizeof(float)];
} floatBytes;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void updateHands();
        void sendHands();
        void drawHands();

		ofxUDPManager udpConnection;

		ofTrueTypeFont  mono;
		ofTrueTypeFont  monosm;
		vector<ofPoint> stroke;
    
        vector<ofPoint> hands;
};

