/*
 AHAS - Arduino Home Automation System
 
 Components:
   
   1. Web Server
     - A simple web server that shows the value of the attached sensors
     using an Arduino Wiznet Ethernet shield.
   2. DHT11 Temperature and humidity sensor   
   3. BMP085 temperature and barometric pressure sensor
   
 
  Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * DHT11 sensor attached to pin 2
 * BMP085 sensor attached to I2C port
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 12 Jan 2015
 by Orbán Balázs
 modified 13 Jan 2015
 by Orbán Balázs
 
 */

/***********************************************************************************************************/
/*** Include ***/
/***********************************************************************************************************/
/** Ethernet Shield **/
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SD.h>

#include <Time.h> 

#include "avr/pgmspace.h" 
#include <EEPROM.h>

/** Webduino **/
#include <WebServer.h>

/** DHT11 sensor **/
#include <dht11.h>

/** BMP085 sensor **/
#include <Wire.h>
#include <BMP085.h>

/** MS5611 sensor **/
#include <MS5611.h>

/** DS1302 Real-time Clock **/
#include <DS1302.h>

/***********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/

void init_UART();
void init_SD();
void init_NetSetup();
void init_Webduino();
void init_DHT11();
void init_BMP085(); 
void init_MS5611();
void init_NTP();
void init_DS1302();
void init_LCD();
void init_OS();
void init_Relay();
void init_IO();
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
  init_IO();
  init_Relay();
  init_LCD();
  init_DHT11();
  init_BMP085(); 
  init_MS5611();
  init_SD();
  init_NetSetup();
  init_Webduino();
  init_NTP();
  init_DS1302();
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
