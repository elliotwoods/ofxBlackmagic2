#include "DeckLink.h"
#include "ofAppGLFWWindow.h"

#include "ofxBlackMagic/Utils.h"

namespace ofxMachineVision {
	namespace Device {
		//----------
		DeckLink::DeckLink() {
			this->input = nullptr;
			this->referenceCount = 0;

			this->frameIndex = 0;
			this->markFrameNew = false;
		}

		//----------
		string DeckLink::getTypeName() const {
			return "DeckLink";
		}

		//----------
		_BMDDisplayMode DeckLink::getDisplayMode() const {
			return (_BMDDisplayMode)this->displayMode;
		}

		//----------
		Specification DeckLink::open(shared_ptr<Base::InitialisationSettings> initialisationSettings) {
			auto settings = this->getTypedSettings<InitialisationSettings>(initialisationSettings);

			auto devices = ofxBlackmagic::Iterator::getDeviceList();
			if (devices.empty()) {
				throw(ofxMachineVision::Exception("No DeckLink devices available"));
			}
			if (devices.size() <= (unsigned int)settings->deviceID) {
				string str = "deviceID [" + ofToString(settings->deviceID) + "] out of range. [" + ofToString(devices.size()) + "] devices available";
				throw(ofxMachineVision::Exception(str));
			}
			this->device = devices[settings->deviceID];
			int width, height;

			this->displayMode = static_cast<_BMDDisplayMode>(settings->displayMode.get());

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
			
			this->frameIndex = 0;

			Specification specification(CaptureSequenceType::Continuous
				, width
				, height
				, "BlackMagic"
				, device.modelName);

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
		}

		//---------
#if defined(_WIN32)
		HRESULT STDMETHODCALLTYPE DeckLink::VideoInputFormatChanged(unsigned long, IDeckLinkDisplayMode*, unsigned long) {
			return S_OK;
		}
#elif defined(__APPLE_CC__)
		HRESULT STDMETHODCALLTYPE DeckLink::VideoInputFormatChanged(BMDVideoInputFormatChangedEvents notificationEvents, IDeckLinkDisplayMode *newDisplayMode, BMDDetectedVideoInputFormatFlags detectedSignalFlags) {
			return S_OK;
		}
#endif

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
				auto frame = FramePool::X().getAvailableAllocatedFrame(width, height, ofPixelFormat::OF_PIXELS_GRAY);

				//copy bytes out from frame
				unsigned char * yuvBytes = nullptr;
				CHECK_ERRORS(videoFrame->GetBytes((void**)& yuvBytes), "Failed to pull bytes from incoming video frame");

				//copy UYVY -> YY
				auto out = frame->getPixels().getPixels();
				for (int i = 0; i < pixelCount; i++) {
					//this method seems to be auto-SIMD optimised
					out[i] = yuvBytes[i * 2 + 1];
				}

				//timestamp
				{
					BMDTimeValue timeValue;
					BMDTimeValue frameDuration;
					BMDTimeScale timeScale = 1e6;
					videoFrame->GetStreamTime(&timeValue, &frameDuration, timeScale);
					frame->setTimestamp(chrono::microseconds(timeValue));
				}

				frame->setFrameIndex(this->frameIndex++);

				//alert the grabber
				this->onNewFrame(frame);
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
#if defined(_WIN32)
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
#elif defined(__APPLE_CC__)
#define CompareREFIID(iid1, iid2)	(memcmp(&iid1, &iid2, sizeof(REFIID)) == 0)
			CFUUIDBytes iunknown = CFUUIDGetUUIDBytes(IUnknownUUID);
			if (CompareREFIID(iid, iunknown))
			{
				*ppv = this;
				AddRef();
				result = S_OK;
			}
			else if (CompareREFIID(iid, IID_IDeckLinkInputCallback))
			{
				*ppv = (IDeckLinkInputCallback*)this;
				AddRef();
				result = S_OK;
			}
#endif
			return result;
		}

		//---------
#if defined(_WIN32)
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
#elif defined(__APPLE_CC__)
		ULONG STDMETHODCALLTYPE DeckLink::AddRef(void) {
			return OSAtomicIncrement32(&referenceCount);
		};
		
		//---------
		ULONG STDMETHODCALLTYPE DeckLink::Release(void) {
			int32_t		newRefValue;
			
			newRefValue = OSAtomicDecrement32(&referenceCount);
			if (newRefValue == 0)
			{
				delete this;
				return 0;
			}
			
			return newRefValue;
		};
#endif
	}
}