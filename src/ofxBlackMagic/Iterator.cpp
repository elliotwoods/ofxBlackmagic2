#include "Iterator.h"

namespace ofxBlackmagic {
	//---------
	DeviceList Iterator::getDeviceList() {
		IDeckLinkIterator* iterator;
		IDeckLink* device;
		DeviceList deviceList;
		auto result = CoCreateInstance(CLSID_CDeckLinkIterator,NULL,CLSCTX_ALL,IID_IDeckLinkIterator,(void**)& iterator);
		
			//comment this out if not yet implemented
			and for mac or linux instead:
			IDeckLinkIterator *deckLinkIterator = CreateDeckLinkIteratorInstance();
			where return of NULL means fail
			also this should be in CoManager not in this cpp
			
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
