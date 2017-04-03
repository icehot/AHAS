// OS.h

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
void Task_Display_Callback();
void Task_Button_Callback();
void Task_Webduino_Callback();
void Task_Log_Callback();
void Task_RenewDHCP_Callback();
void Task_TimeSync_Callback();
void Task_ThingSpeak_Callback();

/** Task Definitions **/

extern Task Task_Init;
extern Task Task_Acquisition;
extern Task Task_Display;
extern Task Task_Button;
extern Task Task_Webduino;
extern Task Task_Log;
extern Task Task_RenewDHCP;
extern Task Task_TimeSync;
extern Task Task_ThingSpeak;

#endif

