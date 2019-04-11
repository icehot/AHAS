/** AHAS Configuration File **/

#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

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
//#define USE_SD
#define USE_RELAY
#define USE_RGB
#define USE_PIR
#define USE_SOUND_DETECT
#define USE_ANALOG_BTN
#define USE_BACKLIGHT
#define USE_CONTRAST
//#define USE_M590
#define USE_MFRC522
#define USE_MQ2
#define USE_WATERSENSOR
#define USE_SPEAKER
#define USE_LIGHTSENSOR

/* SW Components */

//#define USE_NTP
#define USE_WEBDUINO
#define USE_SERIAL_MONITOR
//#define USE_SYS_LOG
#define USE_THINGSPEAK
//#define USE_RUNTIME

/* Cross dependency check*/
#ifndef USE_ETH_SHIELD
#ifdef USE_NTP
    #error "NTP is enabled without ETH Shield"
#endif
#ifdef USE_WEBDUINO
    #error "WEBDUINO is enabled without ETH Shield"
#endif

#ifdef USE_THINGSPEAK
    #error "ThingSpeak is enabled without ETH Shield"
#endif

#endif 

#ifndef USE_SD
#ifdef USE_SYS_LOG
#error "Syslog enabled without SD card support"
#endif
#endif

/***********************************************************************************************************/
/** PIN configuration **/
/***********************************************************************************************************/
#ifdef USE_LCD
#define PIN_LCD_RS 48
#define PIN_LCD_EN 49 
#define PIN_LCD_D4 43
#define PIN_LCD_D5 42 
#define PIN_LCD_D6 41 
#define PIN_LCD_D7 40 
#endif

#ifdef USE_BACKLIGHT
#define PIN_BACKLIGHT 6
#define DEFAULT_BACKLIGHT 128
#endif

#ifdef USE_CONTRAST
#define PIN_CONTRAST 7
#define DEFAULT_CONTRAST 40
#endif

#ifdef USE_RELAY
#define PIN_RELAY1 23
#define PIN_RELAY2 25
#define PIN_RELAY3 27
#define PIN_RELAY4 29
#endif 

#ifdef USE_DHT11
#define PIN_DHT11 37
#endif

#ifdef USE_SOUND_DETECT
#define PIN_SOUND_DETECT 2
#endif 

#ifdef USE_PIR
#define PIN_PIR_SENSOR 47
#endif

#ifdef USE_RGB
#define PIN_RGBLED_R 46
#define PIN_RGBLED_G 45
#define PIN_RGBLED_B 44
#endif

#ifdef USE_DS1302
#define PIN_DS1302_SCLK   35    // Arduino pin for the Serial Clock
#define PIN_DS1302_IO     33    // Arduino pin for the Data I/O
#define PIN_DS1302_CE     31    // Arduino pin for the Chip Enable
#endif

#ifdef USE_M590
#define PIN_M590_ONOFF 3
#endif 

#ifdef USE_MFRC522
#define PIN_MFRC522_RST_PIN 36
#define PIN_MFRC522_CS_PIN 39
#define PIN_MFRC522_IRQ_PIN 3
#endif 

#ifdef USE_ANALOG_BTN
#define PIN_ANALOG_BUTTON A15

#define AN_BTN_TOLERANCE 30

#define BTNULL        993   //NOBUTTON  
#define UL_BTNULL     BTNULL + AN_BTN_TOLERANCE   //NOBUTTON  Upper Limit
#define LL_BTNULL     BTNULL - AN_BTN_TOLERANCE   //NOBUTTON  Lower Limit

#define BTU           864   //UP 
#define UL_BTU        BTU + AN_BTN_TOLERANCE  //UP Upper Limit
#define LL_BTU        BTU - AN_BTN_TOLERANCE  //UP Lower Limit

#define BTL           727   //RIGTH 
#define UL_BTL        BTL + AN_BTN_TOLERANCE   //RIGTH Upper Limit
#define LL_BTL        BTL - AN_BTN_TOLERANCE  //RIGTH Lower Limit

#define BTC           588   //CONFIRM
#define UL_BTC        BTC + AN_BTN_TOLERANCE   //CONFIRM Upper Limit
#define LL_BTC        BTC - AN_BTN_TOLERANCE  //CONFIRM Lower Limit

#define BTR           445   //LEFT 
#define UL_BTR        BTR + AN_BTN_TOLERANCE   //LEFT Upper Limit
#define LL_BTR        BTR - AN_BTN_TOLERANCE  //LEFT Lower Limit

#define BTD           270   //DOWN 
#define UL_BTD        BTD + AN_BTN_TOLERANCE   //DOWN Upper Limit
#define LL_BTD        BTD - AN_BTN_TOLERANCE  //DOWN Lower Limit

#define BTE           30   //ESCAPE 
#define UL_BTE        BTE + AN_BTN_TOLERANCE   //ESCAPE Upper Limit
#define LL_BTE        BTE - AN_BTN_TOLERANCE  //ESCAPE Lower Limit
#endif

#ifdef USE_ETH_SHIELD
#define PIN_ETH_CS 10 //ETH chip select
#endif

//#ifdef USE_SD
#define PIN_SD_CS 4 //SD card chip select
//#endif

#ifdef USE_LIGHTSENSOR
#define PIN_LIGHTSENSOR A14
#endif

#ifdef USE_MQ2
#define PIN_MQ2 A6
#endif

#ifdef USE_WATERSENSOR
#define PIN_WATERSENSOR A7 
#endif

#ifdef USE_SPEAKER
#define PIN_SPEAKER 5 
#endif
#define PIN_SPI_CS 53 //default chip select
#define PIN_BUILTIN_LED 13 // Built-in Light Emitting Diode

#endif