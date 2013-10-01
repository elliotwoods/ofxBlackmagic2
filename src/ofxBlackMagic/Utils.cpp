#include "Utils.h"
#include <iostream>

namespace ofxBlackmagic {
	namespace Utils {
#pragma mark CoManager
		//---------
		CoManager CoManagerInstance = CoManager();

		//---------
		CoManager::CoManager() {
			auto result = CoInitialize(NULL);
			if (FAILED(result)) {
				throw(std::exception("ofxBlackmagic failed to initialise COM. Now quitting."));
			}
			CHECK_ERRORS(CoCreateInstance(CLSID_CDeckLinkVideoConversion, NULL, CLSCTX_ALL, IID_IDeckLinkVideoConversion, (void**)&this->videoConverter), "Failed to create video conversion instance");
		}

		//---------
		CoManager::~CoManager() {
			this->videoConverter->Release();
		}

		//---------
		IDeckLinkVideoConversion& CoManager::getVideoConverter() {
			return *this->videoConverter;
		}

#pragma mark functions
		//---------
		std::string BSTRToString(BSTR& input) {
			auto longVersion = std::wstring(input);
			return std::string(longVersion.begin(), longVersion.end());
		}
	}
}