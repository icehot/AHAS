/** BMP085 sensor **/

#ifndef _BMP085_h
#define _BMP085_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef USE_BMP085
	void init_BMP085();
	void read_BMP085();
#endif

#endif

