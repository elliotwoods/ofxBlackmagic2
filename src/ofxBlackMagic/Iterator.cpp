#include "Iterator.h"

namespace ofxBlackMagic {
	//---------
	DeviceList Iterator::getDeviceList() {
		IDeckLinkIterator* iterator;
		IDeckLink* device;
		DeviceList deviceList;
		auto result = CoCreateInstance(CLSID_CDeckLinkIterator,NULL,CLSCTX_ALL,IID_IDeckLinkIterator,(void**)& iterator);
		while(result == S_OK && iterator->Next(&device) == S_OK)
		{
			DeviceDefinition deviceDefinition;

			BSTR displayName, modelName;

			device->GetDisplayName(&displayName);
			device->GetModelName(&modelName);

			deviceDefinition.device = device;
			deviceDefinition.displayName = Utils::BSTRToString(displayName);
			deviceDefinition.modelName = Utils::BSTRToString(modelName); 
			deviceList.push_back(deviceDefinition);
		}
		return deviceList;
	}
}