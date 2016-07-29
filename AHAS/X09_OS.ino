
void WebduinoServerLoop();
void calcRunTime();
void saveDataToLog();

void init_OS()
{
  TimeStamps.task1s = 0;
  TimeStamps.task2s = 0;
  TimeStamps.task1m = 0;
}

void OS_taskIdle()
{
  // renew DHCP lease
  renewDHCP(eeprom_config.dhcp_refresh_minutes);

  TimeStamps.webStart = millis();
  WebduinoServerLoop();
  TimeStamps.webEnd = millis();
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
    read_DHT11();
    TimeStamps.dht11 = millis();
    read_BMP085(); 
    TimeStamps.bmp085 = millis();
    read_MS5611();
    TimeStamps.ms5611 = millis();
    read_DS1302();
    //read_time(); 
    TimeStamps.ds1302 = millis();

    analogWrite(PIN_RGBLED_R, DataPool.RGB_Red);
    analogWrite(PIN_RGBLED_G, DataPool.RGB_Green);
    analogWrite(PIN_RGBLED_B, DataPool.RGB_Blue);
  }
}

void OS_task2s()
{
  if (TimeStamps.current - TimeStamps.task2s > 2000)
  {
    TimeStamps.task2s = TimeStamps.current;
    updateLCD();
  }
}
void OS_task1m()
{
  if (TimeStamps.current - TimeStamps.task1m > 60000)
  {
    TimeStamps.task1m = TimeStamps.current;
    saveDataToLog();
  }
}

void OS_calcRunTime()
{
  RunTime.dht11  = TimeStamps.dht11  - TimeStamps.cycleStart;
  RunTime.bmp085 = TimeStamps.bmp085 - TimeStamps.dht11;
  RunTime.ms5611 = TimeStamps.ms5611 - TimeStamps.bmp085;
  RunTime.ds1302 = TimeStamps.ds1302 - TimeStamps.ms5611;
  RunTime.total  = TimeStamps.end    - TimeStamps.start;
  RunTime.web    = TimeStamps.webEnd - TimeStamps.webStart; 
}

