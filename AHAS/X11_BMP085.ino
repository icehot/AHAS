/** BMP085 sensor **/
#ifdef USE_BMP085

#include <Wire.h>
#include <BMP085.h>

/** BMP085 sensor **/
#define I2C_ADDRESS 0x77

/** BMP085 sensor **/
BMP085 bmp085;

void init_BMP085()
{
  bmp085.init(I2C_ADDRESS);

  #ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: BMP085 => DONE"));
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: BMP085 => DONE"));
  #endif
}

void read_BMP085()
{
  filterJacob(&DataPool.BMP085_Pressure,(float)bmp085.readPressure(), 4);
  filterJacob(&DataPool.BMP085_Temperature,(float)bmp085.readTemperature(), 4);
}
#endif
