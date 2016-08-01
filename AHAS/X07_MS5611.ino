/** MS5611 sensor **/
MS5611 ms5611;
double referencePressure;

void checkSettings();

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
  DataPool.MS5611_Temperature = ms5611.readTemperature();
  DataPool.MS5611_Pressure = ms5611.readPressure();

  // Calculate altitude
  DataPool.MS5611_AbsAltitude = ms5611.getAltitude(DataPool.MS5611_Pressure);
  DataPool.MS5611_RelAltitude = ms5611.getAltitude(DataPool.MS5611_Pressure, referencePressure);
}

void init_MS5611()
{
  Serial.print("#INIT: MS5611 => ");
   
  if(!ms5611.begin())
  {
    //"Could not find a valid MS5611 sensor, check wiring!");
    Serial.println("FAILED");
  }
  else
  {
    Serial.println("DONE");
  }
  
  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();

  // Check settings
  //checkSettings();
}

void checkSettings()
{
  Serial.print("Oversampling: ");
  Serial.println(ms5611.getOversampling());
}
