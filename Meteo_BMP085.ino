/** BMP085 sensor **/

void init_BMP085()
{
  Wire.begin();
  getCalibrationData();
  dumpCalibrationData();
}

// Below there are the utility functions to get data from the BMP085 sensor.
// read temperature and pressure from sensor
void read_BMP085() 
{
  int  ut= readUT();
  long up = readUP();
  long x1, x2, x3, b3, b5, b6, p;
  unsigned long b4, b7;

  //calculate true temperature
  x1 = ((long)ut - ac6) * ac5 >> 15;
  x2 = ((long) mc << 11) / (x1 + md);
  b5 = x1 + x2;
  DataPool.BMP085_Temperature = (b5 + 8) >> 4;

  //calculate true pressure
  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6 >> 12)) >> 11; 
  x2 = ac2 * b6 >> 11;
  x3 = x1 + x2;
  b3 = (((int32_t) ac1 * 4 + x3)<<oversampling_setting + 2) >> 2;
  x1 = ac3 * b6 >> 13;
  x2 = (b1 * (b6 * b6 >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t) up - b3) * (50000 >> oversampling_setting);
  p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  DataPool.BMP085_Pressure = p + ((x1 + x2 + 3791) >> 4);
}

// read uncompensated temperature value
unsigned int readUT() {
  writeRegister(0xf4,0x2e);
  delay(5); // the datasheet suggests 4.5 ms
  return readIntRegister(0xf6);
}

// read uncompensated pressure value
long readUP() {
  writeRegister(0xf4,0x34+(oversampling_setting<<6));
  delay(pressure_conversiontime[oversampling_setting]);

  unsigned char msb, lsb, xlsb;
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0xf6);  // register to read
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDRESS, 3); // request three bytes
  while(!Wire.available()); // wait until data available
  msb = Wire.read();
  while(!Wire.available()); // wait until data available
  lsb |= Wire.read();
  while(!Wire.available()); // wait until data available
  xlsb |= Wire.read();
  return (((long)msb<<16) | ((long)lsb<<8) | ((long)xlsb)) >>(8-oversampling_setting);
}

void writeRegister(unsigned char r, unsigned char v)
{
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}

// read a 16 bit register
int readIntRegister(unsigned char r)
{
  unsigned char msb, lsb;
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(r);  // register to read
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDRESS, 2); // request two bytes
  while(!Wire.available()); // wait until data available
  msb = Wire.read();
  while(!Wire.available()); // wait until data available
  lsb = Wire.read();
  return (((int)msb<<8) | ((int)lsb));
}

// read an 8 bit register
/*
unsigned char readRegister(unsigned char r)
{
  unsigned char v;
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.send(r);  // register to read
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDRESS, 1); // request a byte
  while(!Wire.available()); // wait until data available
  v = Wire.receive();
  return v;
}
*/

void  getCalibrationData() {
  ac1 = readIntRegister(0xAA);
  ac2 = readIntRegister(0xAC);
  ac3 = readIntRegister(0xAE);
  ac4 = readIntRegister(0xB0);
  ac5 = readIntRegister(0xB2);
  ac6 = readIntRegister(0xB4);
  b1 = readIntRegister(0xB6);
  b2 = readIntRegister(0xB8);
  mb = readIntRegister(0xBA);
  mc = readIntRegister(0xBC);
  md = readIntRegister(0xBE);

}

void  dumpCalibrationData() {
  Serial.println("Reading Calibration Data");
  Serial.print("AC1: ");
  Serial.println(ac1,DEC);
  Serial.print("AC2: ");
  Serial.println(ac2,DEC);
  Serial.print("AC3: ");
  Serial.println(ac3,DEC);
  Serial.print("AC4: ");
  Serial.println(ac4,DEC);
  Serial.print("AC5: ");
  Serial.println(ac5,DEC);
  Serial.print("AC6: ");
  Serial.println(ac6,DEC);
  Serial.print("B1: ");
  Serial.println(b1,DEC);
  Serial.print("B2: ");
  Serial.println(b1,DEC);
  Serial.print("MB: ");
  Serial.println(mb,DEC);
  Serial.print("MC: ");
  Serial.println(mc,DEC);
  Serial.print("MD: ");
  Serial.println(md,DEC);
}

