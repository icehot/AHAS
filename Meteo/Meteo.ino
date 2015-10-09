/*
 Arduino Meteo Station
 
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
#include <Time.h> 
#include <SD.h>


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
/*** Macro definition section ***/
/***********************************************************************************************************/

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

/** Data Pool **/
/* Used for data exchange between webserver and sensors */

struct{
  char* DHT11_Status;
  int DHT11_Temperature;
  int DHT11_Humidity;
  int DHT11_DewPoint;
  int BMP085_Temperature;
  long BMP085_Pressure;
  double MS5611_Temperature;
  long MS5611_Pressure;
  float MS5611_AbsAltitude;
  float MS5611_RelAltitude;
}DataPool;

struct{
  unsigned long cycleStart;
  unsigned long cycleEnd;
  unsigned long dht11;
  unsigned long bmp085;
  unsigned long ms5611;
  unsigned long webStart;
  unsigned long webEnd;
  unsigned long current;
  unsigned long previous;
}TimeStamps;

struct{
  int total;
  int dht11;
  int bmp085;
  int ms5611;
  int web;
}RunTime;
/***********************************************************************************************************/
/*** Arduino initialization ***/
/***********************************************************************************************************/

void setup() 
{
  init_UART();  
  init_SD();
  init_ETH();
  init_DHT11();
  init_BMP085();
  init_MS5611();
  init_NTP();
  init_DS1302();
  TimeStamps.previous = 0;
}

/***********************************************************************************************************/
/*** Arduino Mainloop ***/
/***********************************************************************************************************/
void loop() 
{
  TimeStamps.current = millis();

  if (TimeStamps.current - TimeStamps.previous > 1000)
  {
    TimeStamps.previous = TimeStamps.current;

    TimeStamps.cycleStart = millis();
    read_DHT11();
    TimeStamps.dht11 = millis();
    read_BMP085(); 
    TimeStamps.bmp085 = millis();
    read_MS5611();
    TimeStamps.ms5611 = millis();
    read_DS1302(); 
    calcRunTime();
  }  
  
  webserver();  
}
/***********************************************************************************************************/
/*** Function definition ***/
/***********************************************************************************************************/

