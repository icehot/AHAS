/*
 AHAS - Arduino Home Automation System

  A versatile home automation platform based on Arduino Mega2560 board
 
 Supported HW components:
   
   1.  Ethernet Shield based on Wiznet 5100 
   2.  DHT11 Temperature and humidity sensor   
   3.  BMP085 Temperature and barometric pressure sensor
   4.  MS5611 Temperature and barometric pressure sensor
   5.  DS1302 Real time clock
   6.  HD44780 based 2x16 Char LCD Display
   7.  SD Card support
   8.  Relay module
   9.  RGB LED
   10. Button for restoring factory default settings
   11. Debug Led

Supported SW components:

  1. Network Time Protocol (NTP) synchronisation
  2. Webduino webserver
  3. Serial monitor
  4. SD card logging
  5. EEPROM storage for settings
   
 Created on 12 Jan 2015
 by Orbán Balázs
 modified 02 Aug 2016
 by Orbán Balázs
 */

/***********************************************************************************************************/
/*** Include ***/
/***********************************************************************************************************/

/** Project Configuration **/

/* Comment out the not wanted component`s define */

/* HW */

#define USE_ETH_SHIELD
#define USE_DHT11
#define USE_BMP085
#define USE_MS5611
#define USE_DS1302
#define USE_LCD
#define USE_SD
#define USE_RELAY
#define USE_RGB
#define USE_FACTDEF_BTN
#define USE_DEBUG_LED
#define USE_PIR
#define USE_SOUND_DETECT

/* SW */

#define USE_NTP
#define USE_WEBDUINO
#define USE_SERIAL_MONITOR
#define USE_SYS_LOG


/* Cross dependency check*/
#ifndef USE_ETH_SHIELD
  #ifdef USE_NTP
    #error "NTP is enabled without ETH Shield"
  #endif
  #ifdef USE_WEBDUINO
    #error "WEBDUINO is enabled without ETH Shield"
  #endif
#endif 

#include <TaskScheduler.h>


/**********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/

// Callback methods prototypes

void Task_Init_Callback();
void Task_Acquisition_Callback();
void Task_Display_Callback();
void Task_Webduino_Callback();
void Task_Log_Callback();
void Task_RenewDHCP_Callback();

Scheduler TaskScheduler;

//Tasks
Task Task_Init(TASK_IMMEDIATE, TASK_ONCE, &Task_Init_Callback, &TaskScheduler, true);
Task Task_Acquisition(TASK_SECOND, TASK_FOREVER, &Task_Acquisition_Callback, &TaskScheduler, true);
Task Task_Display(2*TASK_SECOND, TASK_FOREVER, &Task_Display_Callback, &TaskScheduler, false);
Task Task_Webduino(TASK_SECOND/2, TASK_FOREVER, &Task_Acquisition_Callback, &TaskScheduler, false);
Task Task_Log(TASK_MINUTE, TASK_FOREVER, &Task_Log_Callback, &TaskScheduler, false);
Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback, &TaskScheduler, false);

/***********************************************************************************************************/
/*** Arduino initialization ***/
/***********************************************************************************************************/
void setup() 
{
  TaskScheduler.init();
}

/***********************************************************************************************************/
/*** Arduino Mainloop ***/
/***********************************************************************************************************/
void loop() 
{
  TaskScheduler.execute();
}
