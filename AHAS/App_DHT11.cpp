// 
// 
// 

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_DHT11.h"

#include "DHT11.h"

/** DHT11 **/
#ifdef USE_DHT11

/** DHT11 sensor **/
#include <dht11.h>


/** DHT11 sensor **/
DHT11 dht11;


void init_DHT11()
{
    int chk;

    chk = dht11.read(PIN_DHT11);

#ifdef USE_SERIAL_MONITOR
    Serial.print(F("#INIT: DHT11 Version"));
    Serial.print(DHT11LIB_VERSION);
    Serial.print(F(" => "));
#endif
#ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: SOUND DETECT => DONE"));
#endif

    switch (chk)
    {
    case DHTLIB_OK:
        DataPool.DHT11_Status = "OK";
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("DONE"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#INIT: DHT11 => DONE"));
#endif
        break;
    case DHTLIB_ERROR_CHECKSUM:
        DataPool.DHT11_Status = "Checksum error";
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("FAILED (Checksum error)"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#INIT: DHT11 => FAILED (Checksum error)"));
#endif
        break;
    case DHTLIB_ERROR_TIMEOUT:
        DataPool.DHT11_Status = "Time out error";
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("FAILED (Time out error)"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#INIT: DHT11 => FAILED (Time out error)"));
#endif
        break;
    default:
        DataPool.DHT11_Status = "Unknown error";
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("FAILED (Unknown error)"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#INIT: DHT11 => FAILED (Unknown error)"));
#endif
        break;
    }
}

void read_DHT11()
{
    //DHT11 check variable
    int chk;

    chk = dht11.read(PIN_DHT11);

    switch (chk)
    {
    case DHTLIB_OK:
        DataPool.DHT11_Status = "OK";
        break;
    case DHTLIB_ERROR_CHECKSUM:
        DataPool.DHT11_Status = "Checksum error";
        break;
    case DHTLIB_ERROR_TIMEOUT:
        DataPool.DHT11_Status = "Time out error";
        break;
    default:
        DataPool.DHT11_Status = "Unknown error";
        break;
    }

    filterJacob(&DataPool.DHT11_Temperature, (float)dht11.temperature, 4);
    filterJacob(&DataPool.DHT11_Humidity, (float)dht11.humidity, 4);
    DataPool.DHT11_DewPoint = (float)dewPointFast(DataPool.DHT11_Temperature, DataPool.DHT11_Humidity);
}

// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
    double a = 17.271;
    double b = 237.7;
    double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
    double Td = (b * temp) / (a - temp);
    return Td;
}
#endif

