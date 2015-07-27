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
/*** Include start ***/
/***********************************************************************************************************/
/** Ethernet Shield **/
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>


/** DHT11 sensor **/
#include <dht11.h>

/** BMP085 sensor **/
#include <Wire.h>
#include <BMP085.h>

/** MS5611 sensor **/
#include <MS5611.h>

/***********************************************************************************************************/
/*** Macro definition section start ***/
/***********************************************************************************************************/

/** DHT11 sensor **/
#define DHT11PIN 2

/** BMP085 sensor **/
#define I2C_ADDRESS 0x77

/***********************************************************************************************************/
/*** Global Variable definition start ***/
/***********************************************************************************************************/

/** MS5611 sensor **/
MS5611 ms5611;
double referencePressure;

/** DHT11 sensor **/
DHT11 dht11;

/** BMP085 sensor **/
BMP085 bmp085;

/** Ethernet Shield  **/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,177);
EthernetServer server(81);
EthernetClient client;

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



/***********************************************************************************************************/
/*** Arduino initialization start ***/
/***********************************************************************************************************/

void setup() 
{
  init_UART();  
  init_SD();
  init_ETH();
  init_DHT11();
  init_BMP085();
  init_MS5611();
}

/***********************************************************************************************************/
/*** Arduino Mainloop start ***/
/***********************************************************************************************************/
void loop() 
{
  read_DHT11();
  read_BMP085(); 
  read_MS5611();
  webserver();  
}

/***********************************************************************************************************/
/*** Function definition ***/
/***********************************************************************************************************/


