#pragma once

#if defined(_WIN32)
#include "vs/DeckLinkAPI_h.h"
#include <wtypes.h>
#elif defined(__APPLE_CC__)
#include "osx/DeckLinkAPI.h"
#endif

#include <string>
#include <memory>

#ifndef __func__
#define __func__ __FUNCTION__
#endif
#define OFXBM_WARNING ofLogWarning(string(__func__))
#define OFXBM_ERROR ofLogError(string(__func__))
#define OFXBM_FATAL ofLogFatalError(string(__func__))
#define CHECK_ERRORS(x, message) if ((x) != S_OK) { throw std::runtime_error((std::string(__func__) + std::string(": ") +  std::string(message)).c_str()); }

namespace ofxBlackmagic {
	namespace Utils {
		//---------
		class CoManager {
		public:
			static CoManager & X();

			CoManager();
			~CoManager();
			IDeckLinkVideoConversion& getVideoConverter();
		protected:
			IDeckLinkVideoConversion* videoConverter;
			static std::shared_ptr<CoManager> singleton;
		};

		//---------
#if defined(_WIN32)
		std::string toString(BSTR&);
#elif defined(__APPLE_CC__)
		std::string toString(CFStringRef);
#endif
	}
}
