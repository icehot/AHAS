// ThingSpeak.h

#ifndef _THINGSPEAK_h
#define _THINGSPEAK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void init_ThingSpeak();
void init_ThingSpeak();
void updateThingSpeak();

#endif

