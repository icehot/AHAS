void send_Header()
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
}

void send_HTML()
{
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  /* DS1302 - RTC */
  client.print("<b>DS1302 Real-time clock: </b><br />");
  client.print("Sync Status: "); client.print(DataPool.DS1302_SyncStatus);
  client.print("<br />");
  client.print("Date: ");
  client.print(DataPool.DS1302_Year+2000);client.print("/");
  client.print(DataPool.DS1302_Month);client.print("/");
  client.print(DataPool.DS1302_Day);
  client.print("<br />");
  client.print("Time: ");
  client.print(DataPool.DS1302_Hour);client.print(":");client.print(DataPool.DS1302_Minute);client.print(":");client.print(DataPool.DS1302_Second);
  client.print("<br />");
  client.print("Runtime (ms): ");
  client.print(RunTime.ds1302);
  client.print("<br />");
  client.print("<br />");
  
   /* DHT11 sensor */
  client.print("<b>DHT11 Sensor: </b><br />");
  client.print("Status: ");
  client.print(DataPool.DHT11_Status);
  client.print("<br />");
  client.print("Temperature (Celsius): ");
  client.print((float)DataPool.DHT11_Temperature, 2);
  client.print("<br />");
  client.print("Humidity (%): ");
  client.print((float)DataPool.DHT11_Humidity, 2);
  client.print("<br />");
  client.print("Dew PointFast (Celsius): ");
  client.print(DataPool.DHT11_DewPoint);
  client.print("<br />");
  client.print("Runtime (ms): ");
  client.print(RunTime.dht11);
  client.print("<br />");
  client.print("<br />");
  
  /* BMP085 Sensor */
  client.print("<b>BMP085 Sensor: </b><br />");
  client.print("Temperature (Celsius): ");
  client.print((float)DataPool.BMP085_Temperature/10, 2);
  client.print("<br />");
  client.print("Pressure: ");
  client.print((float)DataPool.BMP085_Pressure, 2);
  client.print("<br />");
  client.print("Runtime (ms): ");
  client.print(RunTime.bmp085);
  client.print("<br />");
  client.print("<br />");

  /* MS5611 sensor */
  client.print("<b>MS5611 Sensor: </b><br />");
  client.print("Temperature (Celsius): ");
  client.print((float)DataPool.MS5611_Temperature, 2);
  client.print("<br />");
  client.print("Pressure: ");
  client.print((float)DataPool.MS5611_Pressure, 2);
  client.print("<br />");
  client.print("AbsoluteAltitude (m): ");
  client.print((float)DataPool.MS5611_AbsAltitude, 2);
  client.print("<br />");
  client.print("relativeAltitude (m): ");
  client.print((float)DataPool.MS5611_RelAltitude, 2);
  client.print("<br />");
  client.print("Runtime (ms): ");
  client.print(RunTime.ms5611);
  client.print("<br />");
  client.print("<br />");

  /* Runtime measurement */
  client.print("<b>Runtime: </b><br />");
  client.print("Acquisition (ms): ");
  client.print(RunTime.total);
  client.print("<br />");
  client.print("Web (ms): ");
  client.print(RunTime.web);
  client.print("<br />");
  client.print("<br />");
  
  client.println("</html>");
}
