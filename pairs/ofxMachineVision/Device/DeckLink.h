#pragma once

#include "ofxBlackMagic.h"
#include "ofxMachineVision/Device/Callback.h"

namespace ofxMachineVision {
	namespace Device {
		class DeckLink : public ofxMachineVision::Device::Callback, public IDeckLinkInputCallback {
		public:
			struct InitialisationSettings : Base::InitialisationSettings {
			public:
				InitialisationSettings() {
					add(displayMode.set("Display Mode", bmdModeHD1080p30));
				}

				ofParameter<uint32_t> displayMode;
			};

			DeckLink();
			string getTypeName() const override;
			shared_ptr<Base::InitialisationSettings> getDefaultSettings() const override {
				return make_shared<InitialisationSettings>();
			}

			void setDisplayMode(_BMDDisplayMode);
			_BMDDisplayMode getDisplayMode() const;

			Specification open(shared_ptr<Base::InitialisationSettings> = nullptr) override;
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

			ofxBlackmagic::DeviceDefinition device;
			BMDDisplayMode displayMode = bmdModeHD1080p30;
			IDeckLinkInput * input;
			
#if defined(_WIN32)
			ULONG referenceCount;
#elif defined(__APPLE_CC__)
			int32_t referenceCount;
			#endif
		};
	}
}