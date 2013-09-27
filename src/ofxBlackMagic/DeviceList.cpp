#include "DeviceList.h"

namespace ofxBlackMagic {
	//----------
	std::ostream& operator<<(std::ostream& os, DeviceDefinition& device) {
		os << "Display name : \t" << device.displayName << std::endl;
		os << "Model name : \t" << device.modelName << std::endl;
		return os;
	}
}