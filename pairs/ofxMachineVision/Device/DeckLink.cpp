#include "DeckLink.h"
#include "ofAppGLFWWindow.h"

namespace ofxMachineVision {
	namespace Device {
		//----------
		DeckLink::DeckLink() {
			this->input = nullptr;
			this->referenceCount = 0;
			this->displayMode = bmdModeHD1080p30;

			this->openTime = 0;
			this->frameIndex = 0;
			this->markFrameNew = false;
		}

		//----------
		string DeckLink::getTypeName() const {
			return "DeckLink";
		}

		//----------
		void DeckLink::setDisplayMode(_BMDDisplayMode displayMode) {
			this->displayMode = displayMode;
		}

		//----------
		_BMDDisplayMode DeckLink::getDisplayMode() const {
			return this->displayMode;
		}

		//----------
		Specification DeckLink::open(int deviceID) {
			auto devices = ofxBlackmagic::Iterator::getDeviceList();
			if (devices.empty()) {
				throw(ofxMachineVision::Exception("No DeckLink devices available"));
			}
			if (devices.size() <= (unsigned int)deviceID) {
				throw(ofxMachineVision::Exception("deviceID [" + ofToString(deviceID) + "] out of range. [" + ofToString(devices.size()) + "] devices available"));
			}
			this->device = devices[deviceID];

			int width, height;

			try {
				CHECK_ERRORS(device.device->QueryInterface(IID_IDeckLinkInput, (void**)&this->input), "Failed to query interface");
				CHECK_ERRORS(this->input->SetCallback(this), "Failed to set input callback");

				//find the current display mode
				IDeckLinkDisplayModeIterator * displayModeIterator = 0;
				CHECK_ERRORS(input->GetDisplayModeIterator(&displayModeIterator), "Couldn't get DisplayModeIterator");
				IDeckLinkDisplayMode * displayModeTest = nullptr;
				IDeckLinkDisplayMode * displayModeFound = nullptr;
				while (displayModeIterator->Next(&displayModeTest) == S_OK) {
					if (displayModeTest->GetDisplayMode() == this->displayMode) {
						displayModeFound = displayModeTest;
					}
				}

				if (!displayModeFound) {
					CHECK_ERRORS(S_FALSE, "Cannot find displayMode");
				}
				width = displayModeFound->GetWidth();
				height = displayModeFound->GetHeight();
			}
			catch (std::exception e) {
				throw(ofxMachineVision::Exception(e.what()));
			}
			
			this->openTime = ofGetElapsedTimeMicros();
			this->frameIndex = 0;

			Specification specification(width, height, "BlackMagic", device.modelName);
			specification.addFeature(ofxMachineVision::Feature::Feature_DeviceID);
			specification.addFeature(ofxMachineVision::Feature::Feature_FreeRun);

			return specification;
		}

		//----------
		void DeckLink::close() {
			if (this->input) {
				//lock incoming whilst releasing to make sure we're not filling the frame
				
				this->input->Release();
				this->input = nullptr;
			}
		}

		//----------
		bool DeckLink::startCapture() {
			try {
				CHECK_ERRORS(this->input->EnableVideoInput(this->displayMode, bmdFormat8BitYUV, 0), "Failed to enable video input");
				CHECK_ERRORS(this->input->StartStreams(), "Failed to start streams");
				return true;
			}
			catch (std::exception e) {
				OFXMV_ERROR << e.what();
				return false;
			}
		}

		//---------
		void DeckLink::stopCapture() {
			this->input->StopStreams();
			//ensure the incoming frame isn't locked. should this be done by the grabber?
			while (!this->incomingFrame->lockForWriting()) {
				ofSleepMillis(1);
			}
			this->incomingFrame->unlock();
		}

		//---------
		HRESULT DeckLink::VideoInputFormatChanged(BMDVideoInputFormatChangedEvents, IDeckLinkDisplayMode* displayMode, BMDDetectedVideoInputFormatFlags) {
			return S_OK;
		}

		//---------
		HRESULT DeckLink::VideoInputFrameArrived(IDeckLinkVideoInputFrame* videoFrame, IDeckLinkAudioInputPacket* audioFrame) {
			if (videoFrame == NULL) {
				return S_OK;
			}

			const auto width = videoFrame->GetWidth();
			const auto height = videoFrame->GetHeight();
			const auto pixelCount = width * height;

			try {
				//try to lock for writing
				int tryCount = 0;
				while (!this->incomingFrame->lockForWriting()) {
					ofSleepMillis(1);
					if (tryCount++ > 100) {
						throw(ofxMachineVision::Exception("Timeout processing incoming frame"));
					}
				}

				//check allocation
				if (this->incomingFrame->getPixelsRef().getWidth() != width || this->incomingFrame->getPixelsRef().getHeight() != height) {
					this->incomingFrame->getPixelsRef().allocate(width, height, OF_IMAGE_GRAYSCALE);
				}

				//copy bytes out from frame
				unsigned char * yuvBytes = nullptr;
				CHECK_ERRORS(videoFrame->GetBytes((void**)& yuvBytes), "Failed to pull bytes from incoming video frame");

				//copy UYVY -> YY
				auto out = this->incomingFrame->getPixelsRef().getPixels();
				for (int i = 0; i < pixelCount; i++) {
					//this method seems to be auto-SIMD optimised
					out[i] = yuvBytes[i * 2 + 1];
				}

				this->incomingFrame->setTimestamp(ofGetElapsedTimeMicros() - this->openTime);
				this->incomingFrame->setFrameIndex(this->frameIndex++);

				this->incomingFrame->unlock();

				//alert the grabber
				this->onNewFrame(this->incomingFrame);
			}
			catch (ofxMachineVision::Exception e) {
				OFXMV_ERROR << e.what();
			}

			return S_OK;
		}

		//---------
		HRESULT STDMETHODCALLTYPE DeckLink::QueryInterface(REFIID iid, LPVOID *ppv) {
			HRESULT	result = E_NOINTERFACE;

			if (ppv == NULL)
				return E_INVALIDARG;

			// Initialise the return result
			*ppv = NULL;

			// Obtain the IUnknown interface and compare it the provided REFIID
			if (iid == IID_IUnknown)
			{
				*ppv = this;
				AddRef();
				result = S_OK;
			}
			else if (iid == IID_IDeckLinkInputCallback)
			{
				*ppv = (IDeckLinkInputCallback*)this;
				AddRef();
				result = S_OK;
			}
			else if (iid == IID_IDeckLinkNotificationCallback)
			{
				*ppv = (IDeckLinkNotificationCallback*)this;
				AddRef();
				result = S_OK;
			}

			return result;
		}

		//---------
		ULONG STDMETHODCALLTYPE DeckLink::AddRef(void) {
			return InterlockedIncrement((LONG*)&referenceCount);
		};

		//---------
		ULONG STDMETHODCALLTYPE DeckLink::Release(void) {
			int	newRefValue;

			newRefValue = InterlockedDecrement((LONG*)&referenceCount);
			if (newRefValue == 0)
			{
				//delete this;
				return 0;
			}

			return newRefValue;
		};
	}
}