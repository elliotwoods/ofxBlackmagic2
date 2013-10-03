#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gui.init();
	auto deviceList = ofxBlackmagic::Iterator::getDeviceList();

	A.startCapture(deviceList[0], bmdModePAL);
	B.startCapture(deviceList[1], bmdModePAL);
	mix.allocate(1920, 1080, GL_RGBA);

	gui.add(A, "A");
	gui.add(B, "B");
	this->mixPanel = gui.add(mix, "Mix");

	mixPanel->onDraw += [this] (ofxCvGui::DrawArguments&) {
		float x = ofMap(this->position, -1.0f, 1.0f, 0, this->mixPanel->getWidth(), true);
		ofPushStyle();
		ofSetLineWidth(5.0f);
		ofLine(x, 0, x, this->mixPanel->getHeight());
		ofPopStyle();
	};
	mixPanel->onMouse += [this] (ofxCvGui::MouseArguments& args) {
		this->position = ofMap(args.localNormalised.x, 0, 1.0f, -1.0f, 1.0f);
	};
}

//--------------------------------------------------------------
void ofApp::update(){
	A.update();
	B.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushStyle();
	ofEnableAlphaBlending();
	
	mix.begin();
	ofSetColor(255, 255, 255, 255);
	A.draw(0, 0, mix.getWidth(), mix.getHeight());
	ofSetColor(255, 255, 255, ofMap(position, -1.0f, 1.0f, 0, 255));
	B.draw(0, 0, mix.getWidth(), mix.getHeight());
	mix.end();
	
	ofPopStyle();
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
