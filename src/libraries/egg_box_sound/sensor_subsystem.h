#ifdef __AVR__
#define MSG(name)   static const /*unsigned*/ char name[] __attribute__(( section(".progmem." #name) ))
#else
#define MSG(name)   static const /*unsigned*/ char name[]
#endif


// Enable event manager
/*
#define EVENTMANAGER_DEBUG 1
#include <EventManager.h>
*/


/* Enable debugging support */