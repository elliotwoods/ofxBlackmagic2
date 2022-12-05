#pragma once

#include "Include_DeckLinkSDK.h"

#include "DeviceList.h"

#include <memory>

namespace ofxBlackmagic {
	class Iterator {
	public:
		static DeviceList getDeviceList();
	};
}
