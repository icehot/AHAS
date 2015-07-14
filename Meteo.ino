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

/** BMP085 sensor **/

// From the datasheet the BMP module address LSB distinguishes
// between read (1) and write (0) operations, corresponding to 
// address 0xEF (read) and 0xEE (write).
// shift the address 1 bit right (0xEF or 0xEE), the Wire library only needs the 7
// most significant bits for the address 0xEF >> 1 = 0x77
// 0xEE >> 1 = 0x77

// oversampling setting
// 0 = ultra low power
// 1 = standard
// 2 = high
// 3 = ultra high resolution
const unsigned char oversampling_setting = 3; //oversampling for measurement
const unsigned char pressure_conversiontime[4] = { 
  5, 8, 14, 26 };  // delays for oversampling settings 0, 1, 2 and 3   
  
// sensor registers from the BOSCH BMP085 datasheet
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;

/** MS5611 sensor **/
MS5611 ms5611;
double referencePressure;

/** DHT11 sensor **/
dht11 DHT11;

/** Ethernet Shield  **/
byte mac[] = { 0xBE, 0xD0, 0xBE, 0xD0, 0xBE, 0xD0 };
EthernetServer server(89);
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


