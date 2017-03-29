// DS1302.h

#ifndef _DS1302_h
#define _DS1302_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void init_DS1302();
void read_DS1302();
void autoTimeSync();

#ifdef USE_NTP
void  sync_DS1302withNTP();
#endif

#endif

