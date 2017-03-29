// Var.h

#ifndef _VAR_h
#define _VAR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "AHAS_Config.h"

/**********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/

/** Data Pool **/
/* Used for data exchange between webserver and sensors */

struct{
#ifdef USE_DHT11
    char* DHT11_Status;
    float DHT11_Temperature;
    float DHT11_Humidity;
    float DHT11_DewPoint;
#endif

#ifdef USE_BMP085
    float BMP085_Temperature;
    float BMP085_Pressure;
#endif

#ifdef USE_MS5611
    float MS5611_Temperature;
    float MS5611_Pressure;
    float MS5611_AbsAltitude;
    float MS5611_RelAltitude;
#endif

#ifdef USE_DS1302
    word DS1302_Year;
    byte DS1302_Month;
    byte DS1302_Day;
    byte DS1302_Hour;
    byte DS1302_Minute;
    byte DS1302_Second;
    byte DS1302_DayOfWeek;
    char* DS1302_SyncStatus;
#endif

#ifdef USE_NTP
    word NTP_Year;
    byte NTP_Month;
    byte NTP_Day;
    byte NTP_Hour;
    byte NTP_Minute;
    byte NTP_Second;
    char* NTP_Status;
#endif

#ifdef USE_RGB
    byte RGB_Red;
    byte RGB_Green;
    byte RGB_Blue;
#endif

#ifdef USE_PIR
    byte PIR_State;
#endif

#ifdef USE_SOUND_DETECT
    byte SOUND_State;
#endif

#ifdef USE_BACKLIGHT
    byte LCD_BackLight;// = 128;
#endif

#ifdef USE_CONTRAST
    byte LCD_Contrast;// = 40;
#endif

}DataPool;

#endif