#pragma once
#include "DeckLinkAPI_h.h"
#include "Utils.h"
#include "ofPixels.h"
#include "ofBaseTypes.h"
#include "ofTypes.h"

namespace ofxBlackmagic {
	/// RGB pixels
	class Frame : IDeckLinkVideoFrame, public ofBaseHasPixels {
	public:
		Frame();
		~Frame();
		void allocate(int width, int height);
		void deallocate();
		void copyFromFrame(IDeckLinkVideoFrame*);

		int getWidth() const;
		int getHeight() const;

		ofMutex lock;

		//--
		//ofBaseHasPixels
		//
		ofPixels& getPixels() override;
		const ofPixels& getPixels() const override;
		//
		//--

	protected:
		//--
		//IDeckLinkVideoFrame
		//
		long STDMETHODCALLTYPE GetWidth(void);
		long STDMETHODCALLTYPE GetHeight(void);
		long STDMETHODCALLTYPE GetRowBytes(void);
		BMDPixelFormat STDMETHODCALLTYPE GetPixelFormat(void);
		BMDFrameFlags STDMETHODCALLTYPE GetFlags(void);
		HRESULT STDMETHODCALLTYPE GetBytes(void **buffer);
        HRESULT STDMETHODCALLTYPE GetTimecode(BMDTimecodeFormat format, IDeckLinkTimecode **timecode);
        HRESULT STDMETHODCALLTYPE GetAncillaryData(IDeckLinkVideoFrameAncillary **ancillary);
		//
		//--


		//--
		//IUnknown
		//
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);
		ULONG STDMETHODCALLTYPE AddRef();
		ULONG STDMETHODCALLTYPE Release();
		//
		//--

		ofPixels pixels;
		unsigned char* data;
		IDeckLinkVideoFrameAncillary* ancillary;
		int references;
	};
}