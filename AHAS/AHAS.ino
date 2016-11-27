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
   10. Restoring factory default settings
   11. LCD Backlight setting
   12. LCD Contrast setting
   13. Analog Button support
   14. Sound detection
   15. PIR sensor

Supported SW components:

  1. Network Time Protocol (NTP) synchronisation
  2. Webduino webserver
  3. Serial monitor
  4. SD card logging
  5. EEPROM storage for settings
  6. MenWizz menu on LCD 
  7. ThingSpeak
   
 Project started on 12 Jan 2015
 by Orbán Balázs
 */

/***********************************************************************************************************/
/*** Include ***/
/***********************************************************************************************************/
#define _TASK_TIMECRITICAL
#include <TaskScheduler.h>
#include <MENWIZ.h>

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

Scheduler TaskScheduler;

void Task_Init_Callback();
void Task_Acquisition_Callback();
void Task_Display_Callback();
void Task_Button_Callback();
void Task_Webduino_Callback();
void Task_Log_Callback();
void Task_RenewDHCP_Callback();
void Task_TimeSync_Callback();
void Task_ThingSpeak_Callback();
/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

/** Task Definitions **/
Task Task_Init(TASK_IMMEDIATE, TASK_ONCE, &Task_Init_Callback);
Task Task_Acquisition(1000, TASK_FOREVER, &Task_Acquisition_Callback);
Task Task_Display(1000, TASK_FOREVER, &Task_Display_Callback); //TBD
Task Task_Button(200, TASK_FOREVER, &Task_Button_Callback); //TBD
Task Task_Webduino(500, TASK_FOREVER, &Task_Webduino_Callback);
Task Task_Log(TASK_MINUTE, TASK_FOREVER, &Task_Log_Callback);
Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback);
Task Task_TimeSync(TASK_HOUR, TASK_FOREVER, &Task_TimeSync_Callback);
Task Task_ThingSpeak(TASK_MINUTE, TASK_FOREVER, &Task_ThingSpeak_Callback);

/***********************************************************************************************************/
/*** Arduino initialization ***/
/***********************************************************************************************************/
void setup() 
{
  TaskScheduler.init();
  
  TaskScheduler.addTask(Task_Init);
  TaskScheduler.addTask(Task_Acquisition);
  TaskScheduler.addTask(Task_Button);
  TaskScheduler.addTask(Task_Display);
  TaskScheduler.addTask(Task_Webduino);
  TaskScheduler.addTask(Task_Log);
  TaskScheduler.addTask(Task_RenewDHCP);
  TaskScheduler.addTask(Task_TimeSync);
  TaskScheduler.addTask(Task_ThingSpeak);

  Task_Init.enable();
  Task_Button.enable();
  Task_Acquisition.enable();
  Task_TimeSync.enable();
  Task_ThingSpeak.enable();
}

/***********************************************************************************************************/
/*** Arduino Mainloop ***/
/***********************************************************************************************************/
void loop() 
{
  TaskScheduler.execute();
}

