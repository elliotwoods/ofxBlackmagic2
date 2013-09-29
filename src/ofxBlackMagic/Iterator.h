#pragma once
#include "DeviceList.h"

#include "DeckLinkAPI_h.h"
#include <memory>

namespace ofxBlackMagic {
	class Iterator {
	public:
		static DeviceList getDeviceList();
	};
}