void init_SD()
{
    //SD card initialization
  Serial.print("Initializing SD card...");

  //Default CS port must be set to output
  pinMode(53, OUTPUT);
  
  if (!SD.begin(4)) 
    Serial.println("initialization failed!");
  else
    Serial.println("initialization done.");
}