#include "ofApp.h"
#include <GLFW/glfw3.h>

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true); // this seems to have no effect on windows/64/glfw currently
	ofSetFrameRate(60);

	gui.init();
	auto rootGrid = gui.addGrid();

	auto deviceList = ofxBlackmagic::Iterator::getDeviceList();

	for(auto device : deviceList) {
		auto input = shared_ptr<ofxBlackmagic::Input>(new ofxBlackmagic::Input());

		static int index = 0;
		auto mode = bmdModeHD1080p30; // switch this mode to match the resolution/refresh of your input stream
		input->startCapture(device, mode);
		this->inputs.push_back(input);

		auto panel = make_shared<ofxCvGui::Panels::Draws>(*input);
		panel->setCaption(input->getDevice().modelName);

		panel->onDraw += [input] (ofxCvGui::DrawArguments&) {
			if (input->isFrameNew()) {
				ofxCvGui::Utils::drawText("New frame", 30, 90);
			}
		};

		rootGrid->add(panel);
	}

	gui.addInspector();


	ofxCvGui::InspectController::X().onClear += [this](ofxCvGui::InspectArguments & args) {
		args.inspector->add(make_shared<ofxCvGui::Widgets::LiveValueHistory>("FPS", []() {
			return ofGetFrameRate();
		}));
	};

	ofxCvGui::InspectController::X().clear();
}

//--------------------------------------------------------------
void ofApp::update(){
	for(auto input : this->inputs) {
		input->update();
	}
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
