#pragma once
#include <vector>
#include <string>
#include <ostream>
#include "DeckLinkAPI.h"

#include "Utils.h"

namespace ofxBlackMagic {
	struct DeviceDefinition {
		IDeckLink* device;
		std::string displayName;
		std::string modelName;
	};
	
	std::ostream& operator<<(std::ostream&, DeviceDefinition&);

	class DeviceList : public std::vector<DeviceDefinition> {

	};
}