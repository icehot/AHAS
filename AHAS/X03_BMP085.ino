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
  Serial.println("#INIT: BMP085 => DONE");

  #ifdef USE_SERIAL_MONITOR
    Serial.println("#INIT: BMP085 => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: BMP085 => DONE");
  #endif
}

void read_BMP085()
{
  DataPool.BMP085_Pressure = bmp085.readPressure();
  DataPool.BMP085_Temperature = bmp085.readTemperature();
}
#endif
