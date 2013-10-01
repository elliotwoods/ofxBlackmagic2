#pragma once
#include <vector>
#include <string>
#include <ostream>

#include "Utils.h"

class IDeckLink;

namespace ofxBlackmagic {
	struct DeviceDefinition {
		IDeckLink* device;
		std::string modelName;
	};

	class DeviceList : public std::vector<DeviceDefinition> {

	};

	std::ostream& operator<<(std::ostream&, DeviceDefinition&);
	std::ostream& operator<<(std::ostream&, DeviceDefinition&);
}