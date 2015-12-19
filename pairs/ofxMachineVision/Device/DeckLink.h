#pragma once

#include "../../../addons/ofxBlackMagic2/src/ofxBlackMagic.h"
#include "../../../addons/ofxMachineVision/src/ofxMachineVision/Device/Callback.h"

namespace ofxMachineVision {
	namespace Device {
		class DeckLink : public ofxMachineVision::Device::Callback, public IDeckLinkInputCallback {
		public:
			DeckLink();
			string getTypeName() const override;

			void setDisplayMode(_BMDDisplayMode);
			_BMDDisplayMode getDisplayMode() const;

			Specification open(int deviceID) override;
			void close() override;
			bool startCapture() override;
			void stopCapture() override;

			// IDeckLinkInputCallback interface
#if defined(_WIN32)
			HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(unsigned long, IDeckLinkDisplayMode*, unsigned long) override;
#elif defined(__APPLE_CC__)
			HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(BMDVideoInputFormatChangedEvents notificationEvents, IDeckLinkDisplayMode *newDisplayMode, BMDDetectedVideoInputFormatFlags detectedSignalFlags) override;
#endif
			HRESULT STDMETHODCALLTYPE VideoInputFrameArrived(IDeckLinkVideoInputFrame*, IDeckLinkAudioInputPacket*) override;

			// IUnknown interface
			HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv) override;
			ULONG STDMETHODCALLTYPE AddRef(void) override;
			ULONG STDMETHODCALLTYPE Release(void) override;
		protected:
			int frameIndex;
			bool markFrameNew;
			ofxMachineVision::Microseconds openTime;

			ofxBlackmagic::DeviceDefinition device;
			BMDDisplayMode displayMode;
			IDeckLinkInput * input;
			
#if defined(_WIN32)
			ULONG referenceCount;
#elif defined(__APPLE_CC__)
			int32_t referenceCount;
			#endif
		};
	}
}