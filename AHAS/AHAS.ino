/***********************************************************************************************************/
/** AHAS - Arduino Home Automation System*/
/***********************************************************************************************************/
 /* A versatile home automation platform based on Arduino Mega2560 board
 
 Supported HW components:
   
   1.  Ethernet shield based on Wiznet 5100 
   2.  DHT11 temperature and humidity sensor   
   3.  BMP085 temperature and barometric pressure sensor
   4.  MS5611 temperature and barometric pressure sensor
   5.  DS1302 real time clock
   6.  HD44780 based 2x16 Char LCD display
   7.  SD card support
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
   
 Project started on 12 Jan 2015
 by Orbán Balázs
 */

/***********************************************************************************************************/
/*** Include ***/
/***********************************************************************************************************/
#define _TASK_TIMECRITICAL
#include <TaskScheduler.h>

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

Scheduler TaskScheduler;

void Task_Init_Callback();
void Task_Acquisition_Callback();
void Task_Display_Callback();
void Task_Webduino_Callback();
void Task_Log_Callback();
void Task_RenewDHCP_Callback();

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

/** Task Definitions **/
Task Task_Init(TASK_IMMEDIATE, TASK_ONCE, &Task_Init_Callback);
Task Task_Acquisition(1000, TASK_FOREVER, &Task_Acquisition_Callback);
Task Task_Display(2000, TASK_FOREVER, &Task_Display_Callback);
Task Task_Webduino(500, TASK_FOREVER, &Task_Webduino_Callback);
Task Task_Log(60000, TASK_FOREVER, &Task_Log_Callback);
Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback);


/***********************************************************************************************************/
/*** Arduino initialization ***/
/***********************************************************************************************************/
void setup() 
{
  TaskScheduler.init();

  TaskScheduler.addTask(Task_Init);
  TaskScheduler.addTask(Task_Acquisition);
  TaskScheduler.addTask(Task_Display);
  TaskScheduler.addTask(Task_Webduino);
  TaskScheduler.addTask(Task_Log);
  TaskScheduler.addTask(Task_RenewDHCP);
  
  Task_Init.enable();
  Task_Acquisition.enable();
  Task_RenewDHCP.enable();
  
  //TaskScheduler.startNow();
}

/***********************************************************************************************************/
/*** Arduino Mainloop ***/
/***********************************************************************************************************/
void loop() 
{
  TaskScheduler.execute();
}

