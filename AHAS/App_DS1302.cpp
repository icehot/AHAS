/** DS1302 Real-time Clock **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_DS1302.h"

#ifdef USE_DS1302

#include <DS1302.h>
#include <TimeX.h> 

#define TIME_SYNC_TRESHOLD_SEC 120

DS1302 ds1302;

void init_DS1302()
{
    /* Initialize the date and time */
    ds1302.init(PIN_DS1302_SCLK, PIN_DS1302_IO, PIN_DS1302_CE);
    /* Read the date and time */
    read_DS1302();
    MONITOR_LOG_LN(F("#INIT: DS1302 => DONE"));
}

void read_DS1302()
{
    ds1302.getTimeAndDate((byte*)&DataPool.DS1302_Year, &DataPool.DS1302_Month, &DataPool.DS1302_Day, &DataPool.DS1302_DayOfWeek, &DataPool.DS1302_Hour, &DataPool.DS1302_Minute, &DataPool.DS1302_Second);
}

void autoTimeSync()
{
    time_t ds1302time;
    time_t ntptime;

    tmElements_t ds1302timeElements;

    ds1302.getTimeAndDate((byte*)&DataPool.DS1302_Year, &DataPool.DS1302_Month, &DataPool.DS1302_Day, &DataPool.DS1302_DayOfWeek, &DataPool.DS1302_Hour, &DataPool.DS1302_Minute, &DataPool.DS1302_Second);
    ntptime = now();

    ds1302timeElements.Year = DataPool.DS1302_Year + 30; /* DS1302 year on 2 digit from 2000 , NTP year is from 1970 */
    ds1302timeElements.Month = DataPool.DS1302_Month;
    ds1302timeElements.Day = DataPool.DS1302_Day;
    ds1302timeElements.Hour = DataPool.DS1302_Hour;
    ds1302timeElements.Minute = DataPool.DS1302_Minute;
    ds1302timeElements.Second = DataPool.DS1302_Second;

    /* convert time elements into time_t */
    ds1302time = makeTime(ds1302timeElements);

    if (abs(ds1302time - ntptime) > TIME_SYNC_TRESHOLD_SEC)
    {
        /* The RTC and the NTP are not in synchron */

        MONITOR_LOG_LN(F("#RTC The RTC need to be synchronised"));

        #ifdef USE_NTP
        sync_DS1302withNTP();
        #endif
    }
    else
    {
        /* The RTC and the NTP are in synchron */
    }
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
        ds1302.setTimeAndDate(year(t) - 2000, month(t), day(t), 1, hour(t), minute(t), second(t));

        MONITOR_LOG_LN(F("#DS1302: synchronized to NTP"));
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
}
#endif
#endif
