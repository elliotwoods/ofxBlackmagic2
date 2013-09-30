#pragma once
#include <string>
#include <wtypes.h>

namespace ofxBlackMagic {
	namespace Utils {
		//---------
		class CoManager {
		public:
			CoManager();
			~CoManager();
		};

		//---------
		std::string BSTRToString(BSTR&);

		//---------
		extern CoManager CoManagerInstance;
	}
}