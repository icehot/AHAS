void init_SD()
{
    //SD card initialization
  Serial.print("#INIT: SD Card => ");

  //Default CS port must be set to output
  pinMode(PIN_SPI_CS, OUTPUT);
  
  if (!SD.begin(PIN_SD_CS))
  { 
    Serial.println("FAILED");
    delay(5000);
    softReset();
  }
  else
  {
    Serial.println("DONE");
  }
}

void saveDataToLog()
{
  // make a string for assembling the data to log:
  String dataString = "";

  /* TimeStamp */
  dataString += String(DataPool.DS1302_Year);
  dataString += "/";
  dataString += String(DataPool.DS1302_Month);
  dataString += "/";
  dataString += String(DataPool.DS1302_Day);
  dataString += " ";
  dataString += String(DataPool.DS1302_Hour);
  dataString += ":";
  dataString += String(DataPool.DS1302_Minute);
  dataString += ",";
  /* Temperature */
  dataString += String((float)(DataPool.DHT11_Temperature + (float)DataPool.BMP085_Temperature/10 + (float)DataPool.MS5611_Temperature)/(float)3);
  dataString += ",";
  /* Humidity */
  dataString += String(DataPool.DHT11_Humidity);
  dataString += ",";
  /* Pressure */
  dataString += String(DataPool.MS5611_Pressure);
  dataString += ",";
  /* Pressure */
  dataString += String(DataPool.BMP085_Pressure);
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) 
  {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println("#SD: Datalog saved");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("#SD: Error opening datalog.txt");
  }
}

