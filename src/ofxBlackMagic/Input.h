#pragma once

#include "Utils.h"
#include "Frame.h"

#include "DeviceList.h"
#include "ofMain.h"

namespace ofxBlackmagic {
	class Input : public IDeckLinkInputCallback, public ofBaseUpdates, public ofBaseDraws, public ofBaseHasPixels, public ofBaseHasTexture {
	public:
		Input();
		virtual ~Input();
		
		void startCapture(const DeviceDefinition&, const BMDDisplayMode&);
		void stopCapture();
		void setMode(const BMDDisplayMode&);

		bool isFrameNew() const;
		DeviceDefinition& getDevice();
		Frame & getFrame();

		//--
		//IDeckLinkInputCallback
		//
#if defined(_WIN32)
		HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(unsigned long, IDeckLinkDisplayMode*, unsigned long) override;
#elif defined(__APPLE_CC__)
		HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(BMDVideoInputFormatChangedEvents notificationEvents, IDeckLinkDisplayMode *newDisplayMode, BMDDetectedVideoInputFormatFlags detectedSignalFlags) override;
#endif

		HRESULT STDMETHODCALLTYPE VideoInputFrameArrived(IDeckLinkVideoInputFrame*, IDeckLinkAudioInputPacket*) override;
		
#if defined(_WIN32)
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) override { return S_OK; };
#elif defined(__APPLE_CC__)
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv) override { return S_OK; };
#endif

		ULONG STDMETHODCALLTYPE AddRef(void) override { return ++referenceCount; };
		ULONG STDMETHODCALLTYPE Release(void) override { return --referenceCount; };
		//
		//--


		//--
		//ofBaseUpdates
		//
		void update() override;
		//
		//--


		//--
		//ofBaseDraws
		//
		void draw(float x, float y) const override;
		void draw(float x, float y, float w, float h) const override;
		float getWidth() const override;
		float getHeight() const override;
		//
		//--


		//--
		//ofBaseHasPixels
		//
		ofPixels & getPixels() override;
		const ofPixels & getPixels() const override;
		//
		//--


		//--
		//ofBaseHasTexture
		//
		ofTexture & getTexture() override;
		const ofTexture & getTexture() const override;
		void setUseTexture(bool) override;
		bool isUsingTexture() const override;
		//
		//--

	protected:
		DeviceDefinition device;
		IDeckLinkInput* input;
		Frame videoFrame;
		ofTexture texture;
		bool useTexture;

		enum {Waiting, Running} state;
		bool newFrameReady; // pre-update
		bool isFrameNewFlag; // post-update
		unsigned long referenceCount;
	};
}
