#pragma once

#if defined(_WIN32)
#include "vs/DeckLinkAPI_h.h"
#elif defined(__APPLE_CC__)
#include "osx/DeckLinkAPI.h"
#endif

#include "DeviceList.h"

#include <memory>

namespace ofxBlackmagic {
	class Iterator {
	public:
		static DeviceList getDeviceList();
	};
}
