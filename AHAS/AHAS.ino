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
#include <TaskScheduler.h>

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/
#define _TASK_TIMECRITICAL
Scheduler TaskScheduler;

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

