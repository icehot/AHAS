// DHT11.h

#ifndef _DHT11_h
#define _DHT11_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void init_DHT11();
void read_DHT11();
double dewPointFast(double celsius, double humidity);

#endif

