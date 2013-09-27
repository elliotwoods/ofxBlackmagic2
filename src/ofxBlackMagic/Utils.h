#pragma once
#include "DeckLinkAPI.h"
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