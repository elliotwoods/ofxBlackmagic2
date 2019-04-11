#pragma once

#include "Utils.h"
#include "ofPixels.h"
#include "ofBaseTypes.h"
#include "ofTypes.h"

namespace ofxBlackmagic {
	/// RGB pixels
	class Frame : IDeckLinkVideoFrame, public ofBaseHasPixels {
	public:
		struct Timecode {
			unsigned char hours;
			unsigned char minutes;
			unsigned char seconds;
			unsigned char frames;
		};

		Frame();
		~Frame();

		void allocate(int width, int height);
		void deallocate();
		void copyFromFrame(IDeckLinkVideoFrame*);
		void swapFrame(Frame& fr);

		int getWidth() const;
		int getHeight() const;
		const Timecode & getTimecode() const;

		ofMutex lock;

		//--
		//ofBaseHasPixels
		//
		ofPixels & getPixels() override;
		const ofPixels & getPixels() const override;
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
#if defined(_WIN32)
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);
#elif defined(__APPLE_CC__)
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv);
#endif
		ULONG STDMETHODCALLTYPE AddRef();
		ULONG STDMETHODCALLTYPE Release();
		//
		//--

		ofPixels pixels;
		Timecode timecode;
		unsigned char* data;
		IDeckLinkVideoFrameAncillary* ancillary;
		int references;
	};
}

std::ostream & operator<<(std::ostream &, const ofxBlackmagic::Frame::Timecode &);
