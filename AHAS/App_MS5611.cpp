/** MS5611 sensor **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_MS5611.h"

#ifdef USE_MS5611

#include <MS5611.h>

MS5611 ms5611;
double referencePressure;

void read_MS5611()
{
    /** MS5611 sensor **/
    // Read raw values
    uint32_t rawTemp;
    uint32_t rawPressure;

    /** MS5611 sensor **/

    // Read raw values
    rawTemp = ms5611.readRawTemperature();
    rawPressure = ms5611.readRawPressure();

    // Read true temperature & Pressure
    filterJacob(&DataPool.MS5611_Temperature, (float)ms5611.readTemperature(), 4);
    filterJacob(&DataPool.MS5611_Pressure, (float)ms5611.readPressure(), 4);

    // Calculate altitude
    DataPool.MS5611_AbsAltitude = ms5611.getAltitude(DataPool.MS5611_Pressure);
    DataPool.MS5611_RelAltitude = ms5611.getAltitude(DataPool.MS5611_Pressure, referencePressure);
}

void init_MS5611()
{
    if (!ms5611.begin())
    {
        //"Could not find a valid MS5611 sensor, check wiring!");

		MONITOR_LOG_LN("#INIT: MS5611 => FAILED");
    }
    else
    {
		MONITOR_LOG_LN("#INIT: MS5611 => DONE");
    }

    // Get reference pressure for relative altitude
    referencePressure = ms5611.readPressure();

    // Check settings
    //checkSettings();
}

void checkSettings()
{
#ifdef USE_SERIAL_MONITOR  
    Serial.print(F("Oversampling: "));
    Serial.println(ms5611.getOversampling());
#endif
}
#endif

