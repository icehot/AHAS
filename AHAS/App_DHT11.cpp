/** DHT11 sensor **/
#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_DHT11.h"

#ifdef USE_DHT11

#include <dht11.h>

DHT11 dht11;

void init_DHT11()
{
    int chk;

    chk = dht11.read(PIN_DHT11);

    MONITOR_LOG(F("#INIT: DHT11 => "));

    switch (chk)
    {
    case DHTLIB_OK:
        DataPool.DHT11_Status = "OK";
		MONITOR_LOG_LN(F("DONE"));
        break;
    case DHTLIB_ERROR_CHECKSUM:
        DataPool.DHT11_Status = "Checksum error";
        MONITOR_LOG_LN(F("FAILED (Checksum error)"));
        break;
    case DHTLIB_ERROR_TIMEOUT:
        DataPool.DHT11_Status = "Time out error";
        MONITOR_LOG_LN(F("FAILED (Time out error)"));
        break;
    default:
        DataPool.DHT11_Status = "Unknown error";
        MONITOR_LOG_LN(F("FAILED (Unknown error)"));
        break;
    }
}

void read_DHT11()
{
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

