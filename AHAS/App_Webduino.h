// Webduino.h

#ifndef _WEBDUINO_h
#define _WEBDUINO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void init_Webduino();
void WebduinoServerLoop();

#endif

