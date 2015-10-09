


void calcRunTime()
{
  RunTime.dht11  = TimeStamps.dht11  - TimeStamps.cycleStart;
  RunTime.bmp085 = TimeStamps.bmp085 - TimeStamps.dht11;
  RunTime.ms5611 = TimeStamps.ms5611 - TimeStamps.bmp085;
  RunTime.total  = TimeStamps.ms5611 - TimeStamps.cycleStart;
  RunTime.web    = TimeStamps.webEnd - TimeStamps.webStart; 
}
