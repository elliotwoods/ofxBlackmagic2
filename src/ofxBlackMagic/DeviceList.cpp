#include "DeviceList.h"
#include "DeckLink_c.h"

namespace ofxBlackMagic {
	//----------
	std::ostream& operator<<(std::ostream& os, DeviceDefinition& device) {
		os << "Model name : \t" << device.modelName << std::endl;
		return os;
	}
}