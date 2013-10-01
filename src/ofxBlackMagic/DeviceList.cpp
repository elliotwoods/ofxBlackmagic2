#include "DeviceList.h"

namespace ofxBlackmagic {
	//----------
	std::ostream& operator<<(std::ostream& os, DeviceDefinition& device) {
		os << "Model name : \t" << device.modelName << std::endl;
		return os;
	}
}