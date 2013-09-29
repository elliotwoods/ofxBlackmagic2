#pragma once
#include "DeckLinkAPI_h.h"
#include <string>

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