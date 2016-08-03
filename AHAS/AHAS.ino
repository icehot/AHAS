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

/**********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/

void init_UART();
#ifdef USE_SD 
  void init_SD(); 
#endif
#ifdef USE_ETH_SHIELD
  void init_NetSetup();
#endif
#ifdef USE_WEBDUINO
  void init_Webduino();
#endif
#ifdef USE_DHT11
  void init_DHT11();
#endif
#ifdef USE_BMP085
  void init_BMP085(); 
#endif
#ifdef USE_MS5611
  void init_MS5611();
#endif
#ifdef USE_NTP
  void init_NTP();
#endif
#ifdef USE_DS1302
  void init_DS1302();
#endif
#ifdef USE_LCD
  void init_LCD();
#endif
#ifdef USE_RELAY
  void init_Relay();
#endif 
#ifdef USE_RGB
  void init_RGB();
#endif
#ifdef USE_FACTDEF_BTN
  void init_FACT_DEF_BTN();
#endif
#ifdef USE_DEBUG_LED
  void init_DEBUG_LED();
#endif 
#ifdef USE_PIR
  void init_PIR();
#endif
void init_OS();
void OS_loopStart();
void OS_task1s();
void OS_task2s();
void OS_task1m();
void OS_taskIdle(); 
void OS_calcRunTime();
void OS_loopEnd(); 

/***********************************************************************************************************/
/*** Arduino initialization ***/
/***********************************************************************************************************/
void setup() 
{
  init_UART();
  
  #ifdef USE_DEBUG_LED
    init_DEBUG_LED();
  #endif 

  #ifdef USE_FACTDEF_BTN
    init_FACT_DEF_BTN();
  #endif

  #ifdef USE_RELAY
    init_Relay();
  #endif 

  #ifdef USE_PIR
    init_PIR();
  #endif

  #ifdef USE_RGB
    init_RGB();
  #endif
  
  #ifdef USE_LCD
    init_LCD();
  #endif

  #ifdef USE_DHT11
    init_DHT11();
  #endif

  #ifdef USE_BMP085
    init_BMP085(); 
  #endif

  #ifdef USE_MS5611
    init_MS5611();
  #endif

  #ifdef USE_SD
    init_SD();
  #endif 

  #ifdef USE_ETH_SHIELD
    init_NetSetup();
  #endif
  
  #ifdef USE_WEBDUINO
    init_Webduino();
  #endif
  
  #ifdef USE_NTP
    init_NTP();
  #endif
  
  #ifdef USE_DS1302
    init_DS1302();
  #endif
  
  init_OS();
}

/***********************************************************************************************************/
/*** Arduino Mainloop ***/
/***********************************************************************************************************/
void loop() 
{
  OS_loopStart();
  OS_task1s();
  OS_task2s();
  OS_task1m();
  OS_taskIdle();
  OS_calcRunTime();
  OS_loopEnd(); 
}
