/** Configuration **/

/***********************************************************************************************************/
/** Project Configuration **/
/***********************************************************************************************************/
/* Comment out the not wanted component`s defines */

/* HW Components */

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
#define USE_ANALOG_BTN

/* SW Components */

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

/***********************************************************************************************************/
/** PIN configuration **/
/***********************************************************************************************************/
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
  #define PIN_RESET 33
#endif

#ifdef USE_DEBUG_LED
  #define PIN_DEBUG_LED   34 
#endif

#ifdef USE_SOUND_DETECT
  #define PIN_SOUND_DETECT 42
#endif 

#ifdef USE_PIR
  #define PIN_PIR_SENSOR 43
#endif

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

#ifdef USE_ANALOG_BTN
  #define PIN_ANALOG_BUTTON a15
#endif

#ifdef USE_ETH_SHIELD or USE_SD
  #define PIN_SD_CS 4 //SD card chip select
#endif

#define PIN_SPI_CS 53 //default chip select
