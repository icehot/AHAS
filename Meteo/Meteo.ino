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
/*** Macro definition section ***/
/***********************************************************************************************************/
/** PIN configuration **/

#define PIN_LCD_RS 22
#define PIN_LCD_EN 23
#define PIN_LCD_D4 24
#define PIN_LCD_D5 25
#define PIN_LCD_D6 26
#define PIN_LCD_D7 27

#define PIN_RELAY1 28
#define PIN_RELAY2 29
#define PIN_RELAY3 30
#define PIN_RELAY4 31

#define PIN_DHT11 32

#define PIN_ANALOG_BUTTON a15
#define PIN_SOUND_DETECTOR 42
#define PIN_PIR_SENSOR 43

#define PIN_RGBLED_R 45
#define PIN_RGBLED_G 44
#define PIN_RGBLED_B 46

#define PIN_DS1302_SCLK   47    // Arduino pin for the Serial Clock
#define PIN_DS1302_IO     48    // Arduino pin for the Data I/O
#define PIN_DS1302_CE     49    // Arduino pin for the Chip Enablev

#define PIN_SPI_CS 53 //default chip select
#define PIN_SD_CS 4 //SD card chip select

/*Configuration*/
#define PREFIX ""
#define PORT 89

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/
/*Configuration*/
byte mac[] = { 0xBE, 0xD0, 0xBE, 0xD0, 0xBE, 0xD0 };

const int timeZone = 2;     // Eastern European Time
// NTP Servers:
IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov
unsigned int localPort = 8888;  // local port to listen for UDP packets

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
  word DS1302_Year;
  byte DS1302_Month;
  byte DS1302_Day;
  byte DS1302_Hour;
  byte DS1302_Minute;
  byte DS1302_Second;
  byte DS1302_DayOfWeek;
  char* DS1302_SyncStatus;
  byte RGB_Red;
  byte RGB_Green;
  byte RGB_Blue;
}DataPool;

struct{
  unsigned long cycleStart;
  unsigned long cycleEnd;
  unsigned long dht11;
  unsigned long bmp085;
  unsigned long ms5611;
  unsigned long ds1302;
  unsigned long webStart;
  unsigned long webEnd;
  unsigned long current;
  unsigned long task1s;
  unsigned long task2s;
  unsigned long task1m;
}TimeStamps;

struct{
  int total;
  int dht11;
  int bmp085;
  int ms5611;
  int ds1302;
  int web;
}RunTime;

/***********************************************************************************************************/
/*** Arduino initialization ***/
/***********************************************************************************************************/
void setup() 
{
  init_UART();  
  init_SD();
  init_Webduino();
  init_DHT11();
  init_BMP085(); 
  init_MS5611();
  init_NTP();
  init_DS1302();
  init_LCD();
  init_OS();
  init_Relay();
  init_RGBLED();
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
}
