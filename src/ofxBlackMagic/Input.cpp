#include "Input.h"

namespace ofxBlackmagic {
	//---------
	Input::Input() {
		this->useTexture = true;
		this->newFrameReady = false;
		this->isFrameNewFlag = false;
		this->referenceCount = 1;
		this->state = Waiting;
	}

	//---------
	Input::~Input() {
		this->stopCapture();
	}

	//---------
	void Input::startCapture(const DeviceDefinition& device, const BMDDisplayMode& format) {
		try {
			this->stopCapture();
			this->device = device;
			CHECK_ERRORS(device.device->QueryInterface(IID_IDeckLinkInput, (void**)&this->input), "Failed to query interface");
			CHECK_ERRORS(this->input->SetCallback(this), "Failed to set input callback");
			CHECK_ERRORS(this->input->EnableVideoInput(format, bmdFormat8BitYUV, 0), "Failed to enable video input");
			CHECK_ERRORS(this->input->StartStreams(), "Failed to start streams");
			this->state = Running;
		} catch(std::exception e) {
			OFXBM_ERROR << e.what();
			this->state = Waiting;
		}
	}

	//---------
	void Input::stopCapture() {
		if (this->state != Running) {
			return;
		}
		try {
			CHECK_ERRORS(this->input->StopStreams(), "Failed to stop streams"); 
		} catch (std::exception e) {
			OFXBM_ERROR << e.what();
		}
		this->state = Waiting;
	}

	//---------
	void Input::setMode(const BMDDisplayMode& displayMode) {
		if (this->state != Running) {
			OFXBM_ERROR << "Cannot setMode if the Input is not already running. Please use startCapture first";
			return;
		}
		this->startCapture(this->device, displayMode);
	}

	//---------
	bool Input::isFrameNew() const {
		return this->isFrameNewFlag;
	}
	
	//---------
	DeviceDefinition& Input::getDevice() {
		return this->device;
	}

	//---------
	HRESULT Input::VideoInputFormatChanged(BMDVideoInputFormatChangedEvents, IDeckLinkDisplayMode* displayMode, BMDDetectedVideoInputFormatFlags) {
		return S_OK;
	}

	//---------
	HRESULT Input::VideoInputFrameArrived(IDeckLinkVideoInputFrame* videoFrame, IDeckLinkAudioInputPacket* audioFrame) {
		if (videoFrame == NULL) {
			return S_OK;
		}

		if (this->videoFrame.lock.tryLock(500)) {
			this->videoFrame.copyFromFrame(videoFrame);
			this->videoFrame.lock.unlock();
		}
		else {
			OFXBM_WARNING << "Cannot copy frame data as videoFrame is locked";
		}

		return S_OK;
	}

	//---------
	void Input::update() {
		this->isFrameNewFlag = this->newFrameReady;
		this->newFrameReady = false;
		if (this->isFrameNewFlag) {
			if (this->videoFrame.getWidth() != this->texture.getWidth() || this->videoFrame.getHeight() != this->texture.getHeight()) {
				this->texture.allocate(this->videoFrame.getWidth(), this->videoFrame.getHeight(), GL_RGBA);
			}

			if (this->videoFrame.lock.tryLock(500)) {
				this->texture.loadData(this->videoFrame.getPixels(), this->getWidth(), this->getHeight(), GL_RGBA);
				this->videoFrame.lock.unlock();
			}
		}
	}

	//---------
	void Input::draw(float x, float y) {
		this->draw(x, y, this->getWidth(), this->getHeight());
	}

	//---------
	void Input::draw(float x, float y, float w, float h) {
		this->getTextureReference().draw(x, y, w, h);
	}
	
	//---------
	float Input::getWidth() {
		return this->videoFrame.getWidth();
	}

	//---------
	float Input::getHeight() {
		return this->videoFrame.getHeight();
	}

	//---------
	unsigned char* Input::getPixels() {
		return this->videoFrame.getPixels();
	}

	//---------
	ofPixels& Input::getPixelsRef() {
		return this->videoFrame.getPixelsRef();
	}

	//---------
	ofTexture& Input::getTextureReference() {
		return this->texture;
	}

	//---------
	void Input::setUseTexture(bool useTexture) {
		this->useTexture = useTexture;
	}
}