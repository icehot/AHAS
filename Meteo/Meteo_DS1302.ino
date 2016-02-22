/** DS1302 Real-time Clock **/

/** DS1302 Real-time Clock **/
DS1302 ds1302;

void init_DS1302()
{
    time_t t;
    ds1302.init(PIN_DS1302_SCLK, PIN_DS1302_IO, PIN_DS1302_CE);

    if (timeStatus() != timeNotSet) 
    {/*NTP synchronised */
      Serial.println("NTP is synchronised");
      t = now();
      ds1302.setTimeAndDate(year(t),month(t),day(t),1,hour(t),minute(t),second(t));
    }
    else
    {/*NTP not synchronised */
      Serial.println("NTP is not synchronised");
      ds1302.setTimeAndDate(99,12,31,1,23,58,58);
    }

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
}

void read_DS1302()
{ 
  ds1302.getTimeAndDate((byte*)&DataPool.DS1302_Year, &DataPool.DS1302_Month, &DataPool.DS1302_Day, &DataPool.DS1302_DayOfWeek, &DataPool.DS1302_Hour, &DataPool.DS1302_Minute, &DataPool.DS1302_Second);
}
