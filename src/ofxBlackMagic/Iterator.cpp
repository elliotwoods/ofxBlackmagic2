#include "Iterator.h"

namespace ofxBlackmagic {
	//---------
	DeviceList Iterator::getDeviceList() {
		IDeckLinkIterator* iterator;
		IDeckLink* device;
		DeviceList deviceList;
		auto result = CoCreateInstance(CLSID_CDeckLinkIterator,NULL,CLSCTX_ALL,IID_IDeckLinkIterator,(void**)& iterator);
		while(result == S_OK && iterator->Next(&device) == S_OK)
		{
			DeviceDefinition deviceDefinition;

			BSTR modelName;

			device->GetModelName(&modelName);

			deviceDefinition.device = device;
			deviceDefinition.modelName = Utils::BSTRToString(modelName); 
			deviceList.push_back(deviceDefinition);
		}
		return deviceList;
	}
}