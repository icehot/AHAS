// RunTime.h

#ifndef _RUNTIME_h
#define _RUNTIME_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

typedef struct
{
    unsigned int act;
    unsigned int max;
    unsigned int min;
    float avg;
    float avgNZ;
    unsigned long int count;
    unsigned long int countNZ;

}RunTime_Type;

struct{
    unsigned long TimeStampStart = 0;
    RunTime_Type Task_Init = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    RunTime_Type Task_Acquisition = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    RunTime_Type Task_Display = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    RunTime_Type Task_Webduino = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    RunTime_Type Task_Log = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    RunTime_Type Task_RenewDHCP = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    RunTime_Type Task_ThingSpeak = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
}RunTime;

void startRuntimeMeasurement();
void endRuntimeMeasurement(RunTime_Type* module);
void printRuntTime(RunTime_Type* module);

#endif

