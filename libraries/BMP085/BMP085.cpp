// 
//    FILE: BMP085.cpp
// VERSION: 0.1
// PURPOSE: Barometric Pressure and Temperature Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: 
//
//     URL: 
//
// HISTORY:
// Orban Balazs - Original version
// 
// 

#include "BMP085.h"
/** BMP085 sensor **/

// Below there are the utility functions to get data from the BMP085 sensor.

//Initialize the bmp085 sensor
void BMP085::init(char i2c_address)
{
  this->i2c_address = i2c_address;
  Wire.begin();
  this->getCalibrationData();
  this->dumpCalibrationData();
}

long BMP085::readPressure()
{
	read();
	return this->pressure;
}

int BMP085::readTemperature()
{
	read();
	return this->temperature;
}
	
// read temperature and pressure from sensor
void BMP085::read()
{
  int  ut= this->readUT();
  long up = this->readUP();
  long x1, x2, x3, b3, b5, b6, p;
  unsigned long b4, b7;

  //calculate true temperature
  x1 = ((long)ut - this->ac6) * this->ac5 >> 15;
  x2 = ((long) this->mc << 11) / (x1 + this->md);
  b5 = x1 + x2;
  this->temperature = (b5 + 8) >> 4;

  //calculate true pressure
  b6 = b5 - 4000;
  x1 = (this->b2 * (b6 * b6 >> 12)) >> 11; 
  x2 = this->ac2 * b6 >> 11;
  x3 = x1 + x2;
  b3 = (((int32_t) this->ac1 * 4 + x3)<<this->oversampling_setting + 2) >> 2;
  x1 = this->ac3 * b6 >> 13;
  x2 = (this->b1 * (b6 * b6 >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (this->ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t) up - b3) * (50000 >> this->oversampling_setting);
  p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  
  this->pressure = (p + ((x1 + x2 + 3791) >> 4));
}

// read uncompensated temperature value
int BMP085::readUT() {
  this->writeRegister(0xf4,0x2e);
  delay(5); // the datasheet suggests 4.5 ms
  return this->readIntRegister(0xf6);
}

// read uncompensated pressure value
long BMP085::readUP() {
  this->writeRegister(0xf4,0x34+(this->oversampling_setting<<6));
  delay(pressure_conversiontime[this->oversampling_setting]);

  unsigned char msb, lsb, xlsb;
  Wire.beginTransmission(this->i2c_address);
  Wire.write(0xf6);  // register to read
  Wire.endTransmission();

  Wire.requestFrom(this->i2c_address, 3); // request three bytes
  while(!Wire.available()); // wait until data available
  msb = Wire.read();
  while(!Wire.available()); // wait until data available
  lsb |= Wire.read();
  while(!Wire.available()); // wait until data available
  xlsb |= Wire.read();
  return (((long)msb<<16) | ((long)lsb<<8) | ((long)xlsb)) >>(8-oversampling_setting);
}

void BMP085::writeRegister(unsigned char r, unsigned char v)
{
  Wire.beginTransmission(this->i2c_address);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}

// read a 16 bit register
int BMP085::readIntRegister(unsigned char r)
{
  unsigned char msb, lsb;
  Wire.beginTransmission(this->i2c_address);
  Wire.write(r);  // register to read
  Wire.endTransmission();

  Wire.requestFrom(this->i2c_address, 2); // request two bytes
  while(!Wire.available()); // wait until data available
  msb = Wire.read();
  while(!Wire.available()); // wait until data available
  lsb = Wire.read();
  return (((int)msb<<8) | ((int)lsb));
}

// read an 8 bit register
/*
unsigned char BMP085::readRegister(unsigned char r)
{
  unsigned char v;
  Wire.beginTransmission(this->i2c_address);
  Wire.send(r);  // register to read
  Wire.endTransmission();

  Wire.requestFrom(this->i2c_address, 1); // request a byte
  while(!Wire.available()); // wait until data available
  v = Wire.receive();
  return v;
}
*/

void  BMP085::getCalibrationData() {
  this->ac1 = this->readIntRegister(0xAA);
  this->ac2 = this->readIntRegister(0xAC);
  this->ac3 = this->readIntRegister(0xAE);
  this->ac4 = this->readIntRegister(0xB0);
  this->ac5 = this->readIntRegister(0xB2);
  this->ac6 = this->readIntRegister(0xB4);
  this->b1  = this->readIntRegister(0xB6);
  this->b2  = this->readIntRegister(0xB8);
  this->mb  = this->readIntRegister(0xBA);
  this->mc  = this->readIntRegister(0xBC);
  this->md  = this->readIntRegister(0xBE);
}

void  BMP085::dumpCalibrationData() {
  Serial.println("Reading Calibration Data");
  Serial.print("AC1: ");
  Serial.println(this->ac1,DEC);
  Serial.print("AC2: ");
  Serial.println(this->ac2,DEC);
  Serial.print("AC3: ");
  Serial.println(this->ac3,DEC);
  Serial.print("AC4: ");
  Serial.println(this->ac4,DEC);
  Serial.print("AC5: ");
  Serial.println(this->ac5,DEC);
  Serial.print("AC6: ");
  Serial.println(this->ac6,DEC);
  Serial.print("B1: ");
  Serial.println(this->b1,DEC);
  Serial.print("B2: ");
  Serial.println(this->b1,DEC);
  Serial.print("MB: ");
  Serial.println(this->mb,DEC);
  Serial.print("MC: ");
  Serial.println(this->mc,DEC);
  Serial.print("MD: ");
  Serial.println(this->md,DEC);
}


//
// END OF FILE
//

// From the datasheet the BMP module address LSB distinguishes
// between read (1) and write (0) operations, corresponding to 
// address 0xEF (read) and 0xEE (write).
// shift the address 1 bit right (0xEF or 0xEE), the Wire library only needs the 7
// most significant bits for the address 0xEF >> 1 = 0x77
// 0xEE >> 1 = 0x77

