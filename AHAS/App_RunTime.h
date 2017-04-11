/** RunTime Measurement **/

#ifndef _RUNTIME_h
#define _RUNTIME_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef USE_RUNTIME
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

    #ifdef USE_LCD
    RunTime_Type Task_Display = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    #endif

    #ifdef USE_ANALOG_BTN
    RunTime_Type Task_Button = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    #endif

    #ifdef USE_WEBDUINO
    RunTime_Type Task_Webduino = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    #endif

    #ifdef USE_SYS_LOG
    RunTime_Type Task_Log = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    #endif

    #ifdef USE_ETH_SHIELD
    RunTime_Type Task_RenewDHCP = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    #endif

    #ifdef USE_THINGSPEAK
    RunTime_Type Task_TimeSync = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    #endif

    #ifdef USE_THINGSPEAK
    RunTime_Type Task_ThingSpeak = { 0, 0, 65535, 0.0, 0.0, 0, 0 };
    #endif


}RunTime;

void startRuntimeMeasurement();
void endRuntimeMeasurement(RunTime_Type* module);
void printRuntTime(RunTime_Type* module);

#endif

#endif

