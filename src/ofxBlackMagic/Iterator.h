#pragma once
#include "DeviceList.h"

#include <memory>

namespace ofxBlackMagic {
	class Iterator {
	public:
		static DeviceList getDeviceList();
	};
}