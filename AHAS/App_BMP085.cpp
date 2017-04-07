/** BMP085 sensor **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_BMP085.h"

#ifdef USE_BMP085

#include <Wire.h>
#include <Adafruit_BMP085.h>

/** BMP085 sensor **/
#define I2C_ADDRESS 0x77

/** BMP085 sensor **/
Adafruit_BMP085 bmp;

void init_BMP085()
{
    bmp.begin();
    MONITOR_LOG_LN(F("#INIT: BMP085 => DONE"));
}

void read_BMP085()
{
    filterJacob(&DataPool.BMP085_Pressure, (float)bmp.readPressure(), 4);
    filterJacob(&DataPool.BMP085_Temperature, (float)bmp.readTemperature(), 4);
}
#endif

