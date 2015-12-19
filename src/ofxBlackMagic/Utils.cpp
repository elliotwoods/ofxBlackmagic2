#include "Utils.h"

#include <iostream>

namespace ofxBlackmagic {
	namespace Utils {
#pragma mark CoManager
		//---------
		std::shared_ptr<CoManager> CoManager::singleton = std::shared_ptr<CoManager>();

		//---------
		CoManager & CoManager::X() {
			if (!CoManager::singleton) {
				CoManager::singleton = std::make_shared<CoManager>();
			}
			return *CoManager::singleton;
		}

		//---------
		CoManager::CoManager() {
#if defined(_WIN32)
			auto result = CoInitialize(NULL);
			if (FAILED(result)) {
				throw(std::exception("ofxBlackmagic failed to initialise COM. Now quitting."));
			}
			if (FAILED(CoCreateInstance(CLSID_CDeckLinkVideoConversion, NULL, CLSCTX_ALL, IID_IDeckLinkVideoConversion, (void**)&this->videoConverter))) {
				this->videoConverter = nullptr;
				throw(std::exception("ofxBlackMagic::CoManager : Failed to create video conversion instance"));
			}
			else {
				this->videoConverter->AddRef();
			}
			
#elif defined(__APPLE_CC__)
			videoConverter = CreateVideoConversionInstance();
#endif
		}

		//---------
		CoManager::~CoManager() {
			if (this->videoConverter) {
				this->videoConverter->Release();
			}
		}

		//---------
		IDeckLinkVideoConversion& CoManager::getVideoConverter() {
			return *this->videoConverter;
		}

#pragma mark functions
		//---------
		
#if defined(_WIN32)
		std::string toString(BSTR& input) {
			auto longVersion = std::wstring(input);
			return std::string(longVersion.begin(), longVersion.end());
		}
#elif defined(__APPLE_CC__)
		std::string toString(CFStringRef input) {
			return CFStringGetCStringPtr(input, kCFStringEncodingUTF8);
		}
#endif
	}
}
