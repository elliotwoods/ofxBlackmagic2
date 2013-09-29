#include "Utils.h"
#include <iostream>

namespace ofxBlackMagic {
	namespace Utils {
		//---------
		CoManager CoManagerInstance = CoManager();

		//---------
		CoManager::CoManager() {
			auto result = CoInitialize(NULL);
			if (FAILED(result)) {
				std::cout << "ofxBlackMagic failed to initialise COM. Now quitting.";
				std::exit(1);
			}
		}

		//---------
		CoManager::~CoManager() {
		}

		//---------
		std::string BSTRToString(BSTR& input) {
			auto longVersion = std::wstring(input);
			return std::string(longVersion.begin(), longVersion.end());
		}
	}
}