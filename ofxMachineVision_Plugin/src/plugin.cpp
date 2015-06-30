// This file 'plugin.cpp' in your plugin is like the 'main.cpp' of your application.
// One big difference is that YOU DO HAVE TO edit this file! (unlike main.cpp generally)

// First include the headers for any classes that you want to include
#include "../../../addons/ofxBlackmagic2/src/ofxBlackMagic.h"
#include "../../../addons/ofxBlackmagic2/pairs/ofxMachineVision/Device/DeckLink.h"

#include "ofxPlugin.h"

OFXPLUGIN_INIT_BEGIN(ofxMachineVision::Device::Base)
	OFXPLUGIN_REGISTER(ofxMachineVision::Device::DeckLink);
OFXPLUGIN_INIT_END