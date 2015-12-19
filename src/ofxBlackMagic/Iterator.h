#pragma once

#if defined(_WIN32)
#include "DeckLinkAPI_h.h"
#elif defined(__APPLE_CC__)
#include "DeckLinkAPI.h"
#endif

#include "DeviceList.h"

#include <memory>

namespace ofxBlackmagic {
	class Iterator {
	public:
		static DeviceList getDeviceList();
	};
}