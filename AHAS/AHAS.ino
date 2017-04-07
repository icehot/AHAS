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
#include "AHAS_Config.h"
#include "App_OS.h"

/***********************************************************************************************************/
/*** Arduino initialization ***/
/***********************************************************************************************************/
void setup() 
{
    TaskScheduler.init();

    /** Task assignation to scheduler **/

    TaskScheduler.addTask(Task_Init);

    TaskScheduler.addTask(Task_Acquisition);

    #ifdef USE_ANALOG_BTN
    TaskScheduler.addTask(Task_Button);
    #endif
    
    #ifdef USE_LCD
    TaskScheduler.addTask(Task_Display);
    #endif

    #ifdef USE_WEBDUINO
    TaskScheduler.addTask(Task_Webduino);
    #endif

    #ifdef USE_SYS_LOG
    TaskScheduler.addTask(Task_Log);
    #endif 

    TaskScheduler.addTask(Task_RenewDHCP);
    TaskScheduler.addTask(Task_TimeSync);

    #ifdef USE_THINGSPEAK
    TaskScheduler.addTask(Task_ThingSpeak);
    #endif

    /** Task Enable **/

    Task_Init.enable();
    
    #ifdef USE_ANALOG_BTN
    Task_Button.enable();
    #endif
    
    Task_Acquisition.enable();
    Task_TimeSync.enable();

    #ifdef USE_THINGSPEAK
	Task_ThingSpeak.enableDelayed(TASK_MINUTE);
    #endif
}

/***********************************************************************************************************/
/*** Arduino Mainloop ***/
/***********************************************************************************************************/
void loop() 
{
    TaskScheduler.execute();
}
