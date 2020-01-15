#include "ofApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void ofApp::setup(){
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

    //create the socket and set to send to 127.0.0.1:44409
	udpConnection.Create();
	udpConnection.Connect("127.0.0.1",44409);
	udpConnection.SetNonBlocking(true);
    
    hands.push_back(ofPoint()); hands.push_back(ofPoint());


}

//--------------------------------------------------------------
void ofApp::update(){
    this->updateHands();
    this->sendHands();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(20, 20, 20);
	ofDrawBitmapString("Murmur Hands UDP Send ", 15, 30);
    
    this->drawHands();
}


//--------------------------------------------------------------

void ofApp::updateHands()
{
    float movementSpeed = .05;
    float spacing = 24;
    
    for(int i = 0; i < hands.size(); i++) {
        
        float t = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
        hands[i].x = ofNoise(t, 0);
        hands[i].y = ofNoise(0, t);
    }
}

void ofApp::drawHands()
{

    ofSetColor(20, 20, 20);
    ofNoFill();
    
    for(int i = 0; i < hands.size(); i++) {
        
        ofCircle(hands[i].x*ofGetWidth(), hands[i].y*ofGetHeight(), 20);
        ofCircle(hands[i].x*ofGetWidth(), hands[i].y*ofGetHeight(), 2);
    }
}

//--------------------------------------------------------------

void ofApp::sendHands(){
    //Formatting the message: 'X' <len(c8)> [<xpos(f32),ypos(f32)>] 'x'
    
    //const char* pBuff,	const int iSize
    
    const int coordinatesPerHand = 2;
    const int bytesPerCoordinate = 4;
	const int size = hands.size();
    
    const int iSize = 3 + size*coordinatesPerHand*bytesPerCoordinate;
    char pBuff[3 + 2*2*4];
    
    pBuff[0] = 'X';
    pBuff[1] = 2;
    
    int n = 2;
    
    for (int i = 0; i<hands.size(); i++) {
        floatBytes x,y;
        
        x.f = (float) hands[i].x;
        pBuff[n++] = x.s[0];  pBuff[n++] = x.s[1]; pBuff[n++] = x.s[2];  pBuff[n++] = x.s[3];
        
        y.f = (float) hands[i].y;
        pBuff[n++] = y.s[0];  pBuff[n++] = y.s[1]; pBuff[n++] = y.s[2];  pBuff[n++] = y.s[3];
    }
    
    
    pBuff[n++] = 'x';
    
    
    udpConnection.SendAll(pBuff,iSize);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){


}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    //string message="";
    //message+=ofToString(x)+"|"+ofToString(y);
    //udpConnection.Send(message.c_str(),message.length());
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
