/** OS **/

#ifndef _OS_h
#define _OS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define _TASK_TIMECRITICAL
#include <TaskScheduler.h>
#include "App_OS.h"

extern Scheduler TaskScheduler;

void Task_Init_Callback();
void Task_Acquisition_Callback();

#ifdef USE_LCD
void Task_Display_Callback();
#endif

#ifdef USE_ANALOG_BTN
void Task_Button_Callback();
#endif

#ifdef USE_WEBDUINO
void Task_Webduino_Callback();
#endif

#ifdef USE_SYS_LOG
void Task_Log_Callback();
#endif

void Task_RenewDHCP_Callback();
void Task_TimeSync_Callback();

#ifdef USE_THINGSPEAK
void Task_ThingSpeak_Callback();
#endif

/** Task Declarations **/

extern Task Task_Init;
extern Task Task_Acquisition;

#ifdef USE_LCD
extern Task Task_Display;
#endif

#ifdef USE_ANALOG_BTN
extern Task Task_Button;
#endif

#ifdef USE_WEBDUINO
extern Task Task_Webduino;
#endif

#ifdef USE_SYS_LOG
extern Task Task_Log;
#endif

extern Task Task_RenewDHCP;
extern Task Task_TimeSync;

#ifdef USE_THINGSPEAK
extern Task Task_ThingSpeak;
#endif

#endif

