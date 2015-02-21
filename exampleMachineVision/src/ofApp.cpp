#include "ofApp.h"
#include <GLFW/glfw3.h>

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true); // this seems to have no effect on windows/64/glfw currently
	ofSetFrameRate(60);

	this->grabber.getDeviceTyped()->setDisplayMode(BMDDisplayMode::bmdMode4K2160p30);
	this->grabber.open(0);
	this->grabber.startCapture();

	const auto specification = this->grabber.getDeviceSpecification();
	gui.init();
	gui.add(this->grabber, specification.getManufacturer() + " - " + specification.getModelName());
	gui.addInspector();

	//inspector
	ofxCvGui::InspectController::X().onClear += [this](ofxCvGui::InspectArguments & args) {
		auto inspector = args.inspector;
		inspector->add(make_shared<ofxCvGui::Widgets::LiveValueHistory>("Application FPS", []() {
			return ofGetFrameRate();
		}));
		inspector->add(make_shared<ofxCvGui::Widgets::LiveValueHistory>("Grabber FPS", [this]() {
			return this->grabber.getFps();
		}));

	};
	ofxCvGui::InspectController::X().clear();
}

//--------------------------------------------------------------
void ofApp::update(){
	this->grabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

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
