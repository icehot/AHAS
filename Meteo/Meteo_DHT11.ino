/** DHT11 sensor **/
#define DHT11PIN 2

/** DHT11 sensor **/
DHT11 dht11;

void init_DHT11()
{
  //DHT11 initialization 
  Serial.println("DHT11 Initialization ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}

void read_DHT11()
{
  //DHT11 check variable
  int chk;
  
  chk = dht11.read(DHT11PIN);
          
  switch (chk)
  {
    case DHTLIB_OK: 
      DataPool.DHT11_Status = "OK"; 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
      DataPool.DHT11_Status = "Checksum error"; 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
      DataPool.DHT11_Status = "Time out error"; 
    break;
    default: 
      DataPool.DHT11_Status = "Unknown error"; 
    break;
  }

  DataPool.DHT11_Temperature = dht11.temperature;
  DataPool.DHT11_Humidity    = dht11.humidity;
  DataPool.DHT11_DewPoint    = dewPointFast(dht11.temperature, dht11.humidity);
}

// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
	double Td = (b * temp) / (a - temp);
	return Td;
}
