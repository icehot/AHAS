/** SD Card**/
#include "AHAS_Config.h"
#include "App_SD.h"
#include "App_Var.h"
#include "App_IO.h"

#ifdef USE_SD
#include <SD.h>

void init_SD()
{
    //Default CS port must be set to output
    pinMode(PIN_SPI_CS, OUTPUT);
    digitalWrite(PIN_SPI_CS, HIGH);

    //PIN_ETH_CS

    if (!SD.begin(PIN_SD_CS))
    {
        MONITOR_LN(F("#INIT: SD Card => FAILED"));

        delay(5000);
        softReset();
    }
    else
    {
        MONITOR_LN(F("#INIT: SD Card => DONE"));
        LOG(F("\n### AHAS - Arduino Home Automation System ###\n"));
    }
}

void saveDataToLog()
{
    // make a string for assembling the data to log:
    String dataString = "";

    /* TimeStamp */
    #ifdef USE_DS1302
    dataString += String(DataPool.DS1302_Year);
    dataString += "/";
    dataString += String(DataPool.DS1302_Month);
    dataString += "/";
    dataString += String(DataPool.DS1302_Day);
    dataString += " ";
    dataString += String(DataPool.DS1302_Hour);
    dataString += ":";
    dataString += String(DataPool.DS1302_Minute);
    dataString += ",";
    #endif

    /* Temperature */
    #ifdef USE_MS5611
    dataString += String((float)((float)DataPool.MS5611_Temperature));
    dataString += ",";
    #endif
    #ifdef USE_DHT11
    /* Humidity */
    dataString += String(DataPool.DHT11_Humidity);
    dataString += ",";
    #endif
    /* Pressure */
    #ifdef USE_MS5611
    dataString += String(DataPool.MS5611_Pressure);
    dataString += ",";
    #endif
    /* Pressure */
    #ifdef USE_BMP085
    dataString += String(DataPool.BMP085_Pressure);
    #endif

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.csv", FILE_WRITE);

    if (dataFile)
    {// if the file is available, write to it:
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        MONITOR_LN(F("#SD: Datalog saved"));
    }
    else 
    {// if the file isn't open, pop up an error:
        MONITOR_LN(F("#SD: Error opening datalog.txt"));
    }
}

void add2SysLog(const __FlashStringHelper * entry)
{
    // make a string for assembling the data to log:
    String dataString = "";

    /* TimeStamp */
    #ifdef USE_DS1302
    dataString += String(DataPool.DS1302_Year);
    dataString += "/";
    dataString += String(DataPool.DS1302_Month);
    dataString += "/";
    dataString += String(DataPool.DS1302_Day);
    dataString += " ";
    dataString += String(DataPool.DS1302_Hour);
    dataString += ":";
    dataString += String(DataPool.DS1302_Minute);
    dataString += ",";
    #endif

    dataString += String(entry);

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("system.log", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.println(dataString);
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
        MONITOR_LN(F("#SD: Error opening system.log"));
    }
}

void add2SysLog( const char * entry)
{
    // make a string for assembling the data to log:
    String dataString = "";

    /* TimeStamp */
    #ifdef USE_DS1302
    dataString += String(DataPool.DS1302_Year);
    dataString += "/";
    dataString += String(DataPool.DS1302_Month);
    dataString += "/";
    dataString += String(DataPool.DS1302_Day);
    dataString += " ";
    dataString += String(DataPool.DS1302_Hour);
    dataString += ":";
    dataString += String(DataPool.DS1302_Minute);
    dataString += ",";
    #endif

    dataString += String(entry);

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("system.log", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.println(dataString);
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
        MONITOR_LN(F("#SD: Error opening system.log"));
    }
}

void add2SysLogWOTimeStamp(const char * entry)
{
    // make a string for assembling the data to log:
    String dataString = "";

    dataString += String(entry);

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("system.log", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.println(dataString);
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
        MONITOR_LN(F("#SD: Error opening system.log"));
    }
}

void add2SysLogWOTimeStamp(const __FlashStringHelper * entry)
{
    // make a string for assembling the data to log:
    String dataString = "";

    dataString += String(entry);

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("system.log", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.println(dataString);
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
        MONITOR_LN(F("#SD: Error opening system.log"));
    }
}
#endif
