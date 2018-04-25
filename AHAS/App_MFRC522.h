// App_MFRC522.h

#ifndef _APP_MFRC522_h
#define _APP_MFRC522_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef USE_MFRC522
void init_MFRC522();
void cyclicMFRC522();
#endif


#endif

