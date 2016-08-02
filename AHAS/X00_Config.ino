/** Configuration **/

/***********************************************************************************************************/
/*** Macro definition section ***/
/***********************************************************************************************************/

/** PIN configuration **/
#ifdef USE_LCD
#define PIN_LCD_RS 22
#define PIN_LCD_EN 23
#define PIN_LCD_D4 24
#define PIN_LCD_D5 25
#define PIN_LCD_D6 26
#define PIN_LCD_D7 27
#endif

#ifdef USE_RELAY
#define PIN_RELAY1 28
#define PIN_RELAY2 29
#define PIN_RELAY3 30
#define PIN_RELAY4 31
#endif 

#ifdef USE_DHT11
#define PIN_DHT11 32
#endif

#ifdef USE_FACTDEF_BTN
#define PIN_RESET 33  //Connect a button to this PIN. If the button is hold, an the device is turned on the default ethernet settings are restored.
#endif

#ifdef USE_DEBUG_LED
#define PIN_DEBUG_LED   34 
#endif

#define PIN_ANALOG_BUTTON a15
#define PIN_SOUND_DETECTOR 42
#define PIN_PIR_SENSOR 43

#ifdef USE_RGB
#define PIN_RGBLED_R 45
#define PIN_RGBLED_G 44
#define PIN_RGBLED_B 46
#endif

#ifdef USE_DS1302
#define PIN_DS1302_SCLK   47    // Arduino pin for the Serial Clock
#define PIN_DS1302_IO     48    // Arduino pin for the Data I/O
#define PIN_DS1302_CE     49    // Arduino pin for the Chip Enablev
#endif

#define PIN_SPI_CS 53 //default chip select
#define PIN_SD_CS 4 //SD card chip select


/*Configuration*/


/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

/** Data Pool **/
/* Used for data exchange between webserver and sensors */

struct{
  #ifdef USE_DHT11
  char* DHT11_Status;
  int DHT11_Temperature;
  int DHT11_Humidity;
  int DHT11_DewPoint;
  #endif
  
  #ifdef USE_BMP085
  int BMP085_Temperature;
  long BMP085_Pressure;
  #endif
  
  #ifdef USE_MS5611
  double MS5611_Temperature;
  long MS5611_Pressure;
  float MS5611_AbsAltitude;
  float MS5611_RelAltitude;
  #endif
  
  #ifdef USE_DS1302
  word DS1302_Year;
  byte DS1302_Month;
  byte DS1302_Day;
  byte DS1302_Hour;
  byte DS1302_Minute;
  byte DS1302_Second;
  byte DS1302_DayOfWeek;
  char* DS1302_SyncStatus;
  #endif
  
  #ifdef USE_NTP
  word NTP_Year;
  byte NTP_Month;
  byte NTP_Day;
  byte NTP_Hour;
  byte NTP_Minute;
  byte NTP_Second;
  char* NTP_Status;
  #endif
  
  byte RGB_Red;
  byte RGB_Green;
  byte RGB_Blue;
}DataPool;

struct{
  unsigned long start;
  unsigned long cycleStart;
  unsigned long cycleEnd;
  #ifdef USE_DHT11
  unsigned long dht11;
  #endif
  #ifdef USE_BMP085
  unsigned long bmp085;
  #endif
  #ifdef USE_MS5611
  unsigned long ms5611;
  #endif
  
  #ifdef USE_DS1302
  unsigned long ds1302;
  #endif
  
  #ifdef USE_WEBDUINO
  unsigned long webStart;
  unsigned long webEnd;
  #endif
  
  unsigned long current;
  unsigned long task1s;
  unsigned long task2s;
  unsigned long task1m;
  unsigned long end;
}TimeStamps;

struct{
  int total;
  #ifdef USE_DHT11
  int dht11;
  #endif
  #ifdef USE_BMP085
  int bmp085;
  #endif
  #ifdef USE_MS5611
  int ms5611;
  #endif
  #ifdef USE_DS1302
  int ds1302;
  #endif
  #ifdef USE_WEBDUINO
  int web;
  #endif
}RunTime;
