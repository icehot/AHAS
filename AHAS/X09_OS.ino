/** OS **/
void WebduinoServerLoop();
void calcRunTime();
void saveDataToLog();
void read_time();

void init_OS()
{
  TimeStamps.task1s = 0;
  TimeStamps.task2s = 0;
  TimeStamps.task1m = 0;

  #ifdef USE_SERIAL_MONITOR
    Serial.println("#INIT: OS => DONE ");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: OS => DONE ");
  #endif
}

void OS_taskIdle()
{
  #ifdef USE_ETH_SHIELD
  // renew DHCP lease
  renewDHCP(eeprom_config.dhcp_refresh_minutes);
  #endif

  #ifdef USE_WEBDUINO
  TimeStamps.webStart = millis();
  WebduinoServerLoop();
  TimeStamps.webEnd = millis();
  #endif
}

void OS_loopStart()
{
  TimeStamps.start = TimeStamps.current;
  TimeStamps.current = millis();
}

void OS_loopEnd()
{
  TimeStamps.end = millis();
}

void OS_task1s()
{
  if (TimeStamps.current - TimeStamps.task1s > 1000)
  {
    TimeStamps.task1s = TimeStamps.current;

    TimeStamps.cycleStart = millis();
    #ifdef USE_DHT11
    read_DHT11();
    TimeStamps.dht11 = millis();
    #endif
    #ifdef USE_BMP085
    read_BMP085(); 
    TimeStamps.bmp085 = millis();
    #endif
    #ifdef USE_MS5611
    read_MS5611();
    TimeStamps.ms5611 = millis();
    #endif
    #ifdef USE_NTP
    read_time(); 
    //TimeStamps.ntp = millis();
    #endif
    #ifdef USE_DS1302
    read_DS1302();
    TimeStamps.ds1302 = millis();
    #endif
    //sync_DS1302withNTP(); //Uncomment for DS1302 NTP sync
    #ifdef USE_PIR
    get_PIR_State();
    #endif
    #ifdef USE_SOUND_DETECT
    get_SoundDetect_State();
    #endif
    #ifdef USE_RGB
    analogWrite(PIN_RGBLED_R, DataPool.RGB_Red);
    analogWrite(PIN_RGBLED_G, DataPool.RGB_Green);
    analogWrite(PIN_RGBLED_B, DataPool.RGB_Blue);
    #endif
  }
}

void OS_task2s()
{
  if (TimeStamps.current - TimeStamps.task2s > 2000)
  {
    TimeStamps.task2s = TimeStamps.current;
    #ifdef USE_LCD
    updateLCD();
    #endif
  }
}
void OS_task1m()
{
  if (TimeStamps.current - TimeStamps.task1m > 60000)
  {
    TimeStamps.task1m = TimeStamps.current;
    #ifdef USE_SD
    saveDataToLog();
    #endif
  }
}

void OS_calcRunTime()
{
  /*
  RunTime.dht11  = TimeStamps.dht11  - TimeStamps.cycleStart;
  RunTime.bmp085 = TimeStamps.bmp085 - TimeStamps.dht11;
  RunTime.ms5611 = TimeStamps.ms5611 - TimeStamps.bmp085;
  RunTime.ds1302 = TimeStamps.ds1302 - TimeStamps.ms5611;
  RunTime.total  = TimeStamps.end    - TimeStamps.start;
  RunTime.web    = TimeStamps.webEnd - TimeStamps.webStart; 
  */
}
