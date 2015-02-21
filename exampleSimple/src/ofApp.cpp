#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	auto deviceList = ofxBlackmagic::Iterator::getDeviceList();

	for(auto device : deviceList) {
		auto input = shared_ptr<ofxBlackmagic::Input>(new ofxBlackmagic::Input());

		static int index = 0;
		auto mode = index++ == 0 ? bmdModeHD1080p25 : bmdModeHD1080p24;
		input->startCapture(device, mode);
		this->inputs.push_back(input);
	}

	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
	for(auto input : this->inputs) {
		input->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	float x = 0; float y = 0;
	float width = 1920 / 2; float height = 1080 / 2;
	for(auto input : this->inputs) {
		input->draw(x, y, width, height);
		x += width;
		if (x >= ofGetWidth()) {
			x = 0;
			y += ofGetHeight();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
