/**********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/
void Task_Init_Callback();
void Task_Acquisition_Callback();
void Task_Display_Callback();
void Task_Webduino_Callback();
void Task_Log_Callback();
void Task_RenewDHCP_Callback();

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

/** Task Definitions **/
Task Task_Init(TASK_IMMEDIATE, TASK_ONCE, &Task_Init_Callback, &TaskScheduler, true);
Task Task_Acquisition(TASK_SECOND, TASK_FOREVER, &Task_Acquisition_Callback, &TaskScheduler, true);
Task Task_Display(2*TASK_SECOND, TASK_FOREVER, &Task_Display_Callback, &TaskScheduler, false);
Task Task_Webduino(TASK_SECOND/2, TASK_FOREVER, &Task_Acquisition_Callback, &TaskScheduler, false);
Task Task_Log(TASK_MINUTE, TASK_FOREVER, &Task_Log_Callback, &TaskScheduler, false);
Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback, &TaskScheduler, false);

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

  #ifdef USE_RGB
  byte RGB_Red;
  byte RGB_Green;
  byte RGB_Blue;
  #endif

  #ifdef USE_PIR
  byte PIR_State;
  #endif

  #ifdef USE_SOUND_DETECT
  byte SOUND_State;
  #endif

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
