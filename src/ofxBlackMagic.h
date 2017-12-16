// Add this one here to make sure ofxBlackMagic compile fine 
// with other networking addons
#ifdef _WIN32
#include <winsock2.h>
#endif

#include "ofxBlackmagic/Iterator.h"
#include "ofxBlackmagic/Input.h"
#include "ofxBlackmagic/Output.h"
#include "ofxBlackmagic/DeviceList.h"

#undef small