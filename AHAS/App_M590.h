// App_M590.h

#ifndef _APP_M590_h
#define _APP_M590_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "AHAS_Config.h"

#ifdef USE_M590
void init_M590();
void cyclicM590();
#endif

#endif

