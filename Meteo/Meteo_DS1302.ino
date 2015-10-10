/** DS1302 Real-time Clock **/
#define DS1302_SCLK_PIN   6    // Arduino pin for the Serial Clock
#define DS1302_IO_PIN     7    // Arduino pin for the Data I/O
#define DS1302_CE_PIN     8    // Arduino pin for the Chip Enablev

/** DS1302 Real-time Clock **/
DS1302 ds1302;

void init_DS1302()
{
    time_t t;
    ds1302.init(DS1302_SCLK_PIN, DS1302_IO_PIN, DS1302_CE_PIN);

    if (timeStatus() != timeNotSet) 
    {/*NTP synchronised */
      Serial.println("NTP is synchronised");
      t = now();
      ds1302.setTimeAndDate(year(t)-2000,month(t),day(t),1,hour(t),minute(t),second(t));
    }
    else
    {/*NTP not synchronised */
      Serial.println("NTP is not synchronised");
      ds1302.setTimeAndDate(99,12,31,1,23,58,58);
    }
}

void read_DS1302()
{
    switch (timeStatus())
    {
      case timeNotSet:
      DataPool.DS1302_SyncStatus = "Time not set!";
      break;

      case timeNeedsSync:
      DataPool.DS1302_SyncStatus = "Time needs sync!";
      break;

      case timeSet:
      DataPool.DS1302_SyncStatus = "Time set!";
      break;
      
      default:
      DataPool.DS1302_SyncStatus = "Error!";
      break;
    }
    
    ds1302.getTimeAndDate(&DataPool.DS1302_Year, &DataPool.DS1302_Month, &DataPool.DS1302_Day, &DataPool.DS1302_DayOfWeek, &DataPool.DS1302_Hour, &DataPool.DS1302_Minute, &DataPool.DS1302_Second);
}
