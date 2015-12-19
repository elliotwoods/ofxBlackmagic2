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
		} catch(std::exception& e) {
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
	Frame & Input::getFrame() {
		return this->videoFrame;
	}

	//---------
#if defined(_WIN32)
	HRESULT STDMETHODCALLTYPE Input::VideoInputFormatChanged(unsigned long, IDeckLinkDisplayMode*, unsigned long) {
		return S_OK;
	}
#elif defined(__APPLE_CC__)
	HRESULT STDMETHODCALLTYPE Input::VideoInputFormatChanged(BMDVideoInputFormatChangedEvents notificationEvents, IDeckLinkDisplayMode *newDisplayMode, BMDDetectedVideoInputFormatFlags detectedSignalFlags) {
		return S_OK;
	}
#endif
	
	//---------
	HRESULT Input::VideoInputFrameArrived(IDeckLinkVideoInputFrame* videoFrame, IDeckLinkAudioInputPacket* audioFrame) {
		if (videoFrame == NULL) {
			return S_OK;
		}

		this->videoFrame.lock.lock();
		this->videoFrame.copyFromFrame(videoFrame);
		this->videoFrame.lock.unlock();
		this->newFrameReady = true;

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

			this->videoFrame.lock.lock();
			this->texture.loadData(this->videoFrame.getPixels(), this->getWidth(), this->getHeight(), GL_RGBA);
			this->videoFrame.lock.unlock();
		}
	}

	//---------
	void Input::draw(float x, float y) const {
		this->draw(x, y, this->getWidth(), this->getHeight());
	}

	//---------
	void Input::draw(float x, float y, float w, float h) const {
		this->getTexture().draw(x, y, w, h);
	}
	
	//---------
	float Input::getWidth() const {
		return this->videoFrame.getWidth();
	}

	//---------
	float Input::getHeight() const {
		return this->videoFrame.getHeight();
	}

	//---------
	ofPixels & Input::getPixels() {
		return this->videoFrame.getPixels();
	}

	//---------
	const ofPixels & Input::getPixels() const {
		return this->videoFrame.getPixels();
	}

	//---------
	ofTexture & Input::getTexture() {
		return this->texture;
	}

	//---------
	const ofTexture & Input::getTexture() const {
		return this->texture;
	}

	//---------
	void Input::setUseTexture(bool useTexture) {
		this->useTexture = useTexture;
	}

	//---------
	bool Input::isUsingTexture() const {
		return this->useTexture;
	}
}