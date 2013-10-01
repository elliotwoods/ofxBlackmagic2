#include "Frame.h"
#include <exception>

namespace ofxBlackmagic {
	//----------
	Frame::Frame() {

	}

	//----------
	void Frame::allocate(int width, int height) {
		this->pixels.allocate(width, height, OF_PIXELS_RGBA);
	}

	//----------
	void Frame::copyFromFrame(IDeckLinkVideoFrame* inputFrame) {
		auto inputFormat = inputFrame->GetPixelFormat();
		
		if (inputFrame->GetWidth() != this->GetWidth() || inputFrame->GetHeight() != this->GetHeight()) {
			this->allocate(inputFrame->GetWidth(), inputFrame->GetHeight());
		}
		switch (inputFormat)
		{
		case bmdFormat8BitYUV:
			{
				auto & converter = Utils::CoManagerInstance.getVideoConverter();
				converter.ConvertFrame(inputFrame, this);
			}
			break;
		case bmdFormat8BitARGB:
			{
				void* dataIn;
				inputFrame->GetBytes(&dataIn);
				memcpy(this->getPixels(), dataIn, this->GetRowBytes() * this->GetHeight());
			}
			break;
		case bmdFormat8BitBGRA:
		case bmdFormat10BitYUV:
		case bmdFormat10BitRGB:
		case bmdFormat10BitRGBXLE:
		case bmdFormat10BitRGBX:
			throw(std::exception("Format not supported yet"));
			break;
		default:
			break;
		}

		//inputFrame->GetAncillaryData(&this->ancillary);
	}
	
	//----------
	int Frame::getWidth() {
		return this->pixels.getWidth();
	}

	//----------
	int Frame::getHeight() {
		return this->pixels.getHeight();
	}

	//----------
	unsigned char* Frame::getPixels() {
		return this->pixels.getPixels();
	}

	//----------
	ofPixels& Frame::getPixelsRef() {
		return this->pixels;
	}

	//----------
	long Frame::GetWidth() {
		return this->pixels.getWidth();
	}

	//----------
	long Frame::GetHeight() {
		return this->pixels.getHeight();
	}

	//----------
	long Frame::GetRowBytes() {
		return this->GetWidth() * 4;
	}

	//----------
	BMDPixelFormat Frame::GetPixelFormat() {
		return bmdFormat8BitARGB;
	}

	//----------
	BMDFrameFlags Frame::GetFlags() {
		return 0;
	}

	//----------
	HRESULT Frame::GetBytes(void **buffer) {
		*buffer = this->pixels.getPixels();
		return S_OK;
	}

	//----------
	HRESULT Frame::GetTimecode(BMDTimecodeFormat format, IDeckLinkTimecode **timecode) {
		throw(std::exception("GetTimecode feature not supported at present"));
	}

	//----------
    HRESULT Frame::GetAncillaryData(IDeckLinkVideoFrameAncillary **ancillary) {
		*ancillary = this->ancillary;
		return S_OK;
	}

	//----------
	HRESULT Frame::QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
		return S_OK;
	}

	//----------
	ULONG Frame::AddRef() {
		return ++this->references;
	}

	//----------
	ULONG STDMETHODCALLTYPE Frame::Release() {
		return --this->references;
	}

}