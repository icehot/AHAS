/** BMP085 sensor **/
#define I2C_ADDRESS 0x77

/** BMP085 sensor **/
BMP085 bmp085;

void init_BMP085()
{
  bmp085.init(I2C_ADDRESS);
}

void read_BMP085()
{
  DataPool.BMP085_Pressure = bmp085.readPressure();
  DataPool.BMP085_Temperature = bmp085.readTemperature();
}

