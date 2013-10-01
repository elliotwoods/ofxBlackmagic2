#pragma once
#include "DeckLinkAPI_h.h"
#include "DeviceList.h"

#include <memory>

namespace ofxBlackmagic {
	class Iterator {
	public:
		static DeviceList getDeviceList();
	};
}