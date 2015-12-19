#include "Iterator.h"
#include "ofConstants.h"

namespace ofxBlackmagic {
	//---------
	DeviceList Iterator::getDeviceList() {
		IDeckLinkIterator* iterator;
		IDeckLink* device;
		DeviceList deviceList;

		bool haveItererator;

#ifdef TARGET_WIN32
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		auto result = CoCreateInstance(CLSID_CDeckLinkIterator,NULL,CLSCTX_ALL,IID_IDeckLinkIterator,(void**)& iterator);
		haveItererator = result == S_OK;
#else
		iterator = CreateDeckLinkIteratorInstance();
		haveItererator = iterator != NULL;
#endif
			
		while (haveItererator && iterator->Next(&device) == S_OK)
		{
			DeviceDefinition deviceDefinition;

#ifdef TARGET_WIN32
			BSTR modelName;
#elif defined(TARGET_OSX)
			CFStringRef modelName;
#endif
			device->GetModelName(&modelName);
			deviceDefinition.device = device;
			deviceDefinition.modelName = Utils::toString(modelName);
			deviceList.push_back(deviceDefinition);
		}
		return deviceList;
	}
}
