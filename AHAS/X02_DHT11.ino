/** DHT11 **/
#ifdef USE_DHT11

/** DHT11 sensor **/
#include <dht11.h>


/** DHT11 sensor **/
DHT11 dht11;

double dewPointFast(double celsius, double humidity);

void init_DHT11()
{
  int chk;
  
  chk = dht11.read(PIN_DHT11);  

  Serial.print("#INIT: DHT11 Version: ");
  Serial.print(DHT11LIB_VERSION);
  Serial.print(" => ");
  
  switch (chk)
  {
    case DHTLIB_OK: 
      DataPool.DHT11_Status = "OK"; 
      Serial.println("DONE");
    break;
    case DHTLIB_ERROR_CHECKSUM: 
      DataPool.DHT11_Status = "Checksum error"; 
      Serial.println("FAILED (Checksum error)");
    break;
    case DHTLIB_ERROR_TIMEOUT: 
      DataPool.DHT11_Status = "Time out error";
      Serial.println("FAILED (Time out error)"); 
    break;
    default: 
      DataPool.DHT11_Status = "Unknown error";
      Serial.println("FAILED (Unknown error)"); 
    break;
  }
}

void read_DHT11()
{
  //DHT11 check variable
  int chk;
  
  chk = dht11.read(PIN_DHT11);
          
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
#endif
