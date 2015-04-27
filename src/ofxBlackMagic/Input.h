#pragma once
#include "DeckLinkAPI_h.h"
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
		HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(unsigned long, IDeckLinkDisplayMode*, unsigned long) override;
		HRESULT STDMETHODCALLTYPE VideoInputFrameArrived(IDeckLinkVideoInputFrame*, IDeckLinkAudioInputPacket*) override;
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) override { return S_OK; };
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
		void draw(float x, float y) override;
		void draw(float x, float y, float w, float h) override;
		float getWidth() override;
		float getHeight() override;
		//
		//--


		//--
		//ofBaseHasPixels
		//
		unsigned char* getPixels() override;
		ofPixels& getPixelsRef() override;
		//
		//--


		//--
		//ofBaseHasTexture
		//
		ofTexture& getTextureReference() override;
		void setUseTexture(bool) override;
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
