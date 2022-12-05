#if defined(_WIN32)
#include "vs/DeckLinkAPI.h"
#include <wtypes.h>
#elif defined(__APPLE_CC__)
#include "osx/DeckLinkAPI.h"
#endif