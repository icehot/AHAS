// IO.h

#ifndef _IO_h
#define _IO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef USE_SERIAL_MONITOR
	#define MONITOR(a) Serial.println(F(a));
#else
	#define MONITOR(a)
#endif 

#ifdef USE_SYS_LOG
	#define LOG(a) add2SysLog(F(a));
#else
	#define LOG(a)
#endif

#if (defined(USE_SERIAL_MONITOR) && defined(USE_SYS_LOG))
	#define MONITOR_LOG_LN(a) Serial.println(F(a)); add2SysLog(F(a));
#else
	#if defined(USE_SERIAL_MONITOR)
		#define MONITOR_LOG_LN(a) Serial.println(F(a));
	#else
		#if defined(USE_SYS_LOG)
			#define MONITOR_LOG_LN(a) add2SysLog(F(a));
		#else
			#define MONITOR_LOG_LN(a)
		#endif
	#endif
#endif 

#if (defined(USE_SERIAL_MONITOR) && defined(USE_SYS_LOG))
#define MONITOR_LOG(a) Serial.print(F(a)); add2SysLog(F(a));
#else
#if defined(USE_SERIAL_MONITOR)
#define MONITOR_LOG(a) Serial.print(F(a));
#else
#if defined(USE_SYS_LOG)
#define MONITOR_LOG(a) add2SysLog(F(a));
#else
#define MONITOR_LOG(a)
#endif
#endif
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

