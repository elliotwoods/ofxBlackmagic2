#include "Utils.h"
#include "ofMain.h"

namespace ofxBlackMagic {
	namespace Utils {
		//---------
		CoManager CoManagerInstance = CoManager();

		//---------
		CoManager::CoManager() {
			auto result = CoInitialize(NULL);
			if (FAILED(result)) {
				ofSystemAlertDialog("ofxBlackMagic failed to initialise COM. Now quitting.");
				ofExit();
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