// IO.h

#ifndef _IO_h
#define _IO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void softReset();
void init_LedBuiltIn();
void toggleLedBuiltIn();
void init_UART();

#ifdef USE_BACKLIGHT
void init_BackLight();
void setBackLight(byte value);
#endif

#ifdef USE_CONTRAST
void init_Contrast();
void setContrast(byte value);
#endif

#ifdef USE_ANALOG_BTN
int readAnalogButton();
#endif

#ifdef USE_RGB
void init_RGB();
#endif

#ifdef USE_RELAY
void init_Relay();
#endif

#ifdef USE_PIR
void init_PIR();
void get_PIR_State();
#endif

#ifdef USE_SOUND_DETECT
void ISR_SoundDetect();
void init_SoundDetect();
byte get_SoundDetect_State();
void clear_SoundDetect_State();
#endif

void filterJacob(float * filtValue, float newValue, byte order);

#endif

