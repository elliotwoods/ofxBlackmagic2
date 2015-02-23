#pragma once

#include "../../../addons/ofxBlackMagic2/src/ofxBlackMagic.h"
#include "../../../addons/ofxMachineVision/src/ofxMachineVision/Device/Callback.h"

namespace ofxMachineVision {
	namespace Device {
		class DeckLink : public ofxMachineVision::Device::Callback, public IDeckLinkInputCallback {
		public:
			DeckLink();
			string getTypeName() const override;

			void setDisplayMode(BMDDisplayMode);
			BMDDisplayMode getDisplayMode() const;

			Specification open(int deviceID) override;
			void close() override;
			bool startCapture() override;
			void stopCapture() override;

			// IDeckLinkInputCallback interface
			HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(unsigned long, IDeckLinkDisplayMode*, unsigned long) override;
			HRESULT STDMETHODCALLTYPE VideoInputFrameArrived(IDeckLinkVideoInputFrame*, IDeckLinkAudioInputPacket*) override;

			// IUnknown interface
			HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) override;
			ULONG STDMETHODCALLTYPE AddRef(void) override;
			ULONG STDMETHODCALLTYPE Release(void) override;
		protected:
			int frameIndex;
			bool markFrameNew;
			ofxMachineVision::Microseconds openTime;

			ofxBlackmagic::DeviceDefinition device;
			BMDDisplayMode displayMode;
			IDeckLinkInput * input;

			ULONG referenceCount;
		};
	}
}