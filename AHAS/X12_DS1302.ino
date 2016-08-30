/** DS1302 Real-time Clock **/
#ifdef USE_DS1302

#include <DS1302.h>
#include <Time.h> 

DS1302 ds1302;

void read_DS1302();

void init_DS1302()
{
  /* Initialize the date and time */
  ds1302.init(PIN_DS1302_SCLK, PIN_DS1302_IO, PIN_DS1302_CE);
  /* Read the date and time */
  read_DS1302();
  
  #ifdef USE_SERIAL_MONITOR
    Serial.println("#INIT: DS1302 => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: DS1302 => DONE");
  #endif
}

void read_DS1302()
{ 
  ds1302.getTimeAndDate((byte*)&DataPool.DS1302_Year, &DataPool.DS1302_Month, &DataPool.DS1302_Day, &DataPool.DS1302_DayOfWeek, &DataPool.DS1302_Hour, &DataPool.DS1302_Minute, &DataPool.DS1302_Second);
}

#ifdef USE_NTP
void sync_DS1302withNTP()
{
    time_t t;
    
    switch (timeStatus())
    {
      case timeSet:
      DataPool.DS1302_SyncStatus = "Time set!";
      t = now();
      ds1302.setTimeAndDate(year(t)-2000,month(t),day(t),1,hour(t),minute(t),second(t));
      break;
      
      case timeNotSet:
      DataPool.DS1302_SyncStatus = "Time not set!";
      break;

      case timeNeedsSync:
      DataPool.DS1302_SyncStatus = "Time needs sync!";
      break;
      
      default:
      DataPool.DS1302_SyncStatus = "Error!";
      break;
    }

    #ifdef USE_SERIAL_MONITOR
      Serial.println("#DS1302: synchronized to NTP");
    #endif
    #ifdef USE_SYS_LOG
      add2SysLog("#DS1302: synchronized to NTP");
    #endif
}
#endif
#endif
