// MS5611.h

#ifndef _MS5611_h
#define _MS5611_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void init_MS5611();
void read_MS5611();
void checkSettings();

#endif

