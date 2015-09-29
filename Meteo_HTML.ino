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
  // output the value of each analog input pin

  client.print("Acq Runtime (ms): ");
  client.print(RunTime.total);
  client.print("<br />");

  client.print("Web Runtime (ms): ");
  client.print(RunTime.web);
  client.print("<br />");
  
  client.print("DHT11 Sensor: <br />");
  client.print("Status: ");
  
  client.print(DataPool.DHT11_Status);
  
  client.print("<br />");

  /* DHT11 sensor */
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

  /* BMP085 Sensor */
  client.print("<br />");
  client.print("BMP085 Sensor: ");
  client.print("<br />");
  
  client.print("Temperature (Celsius): ");
  client.print((float)DataPool.BMP085_Temperature/10, 2);
  client.print("<br />");
  
  client.print("Pressure: ");
  client.print((float)DataPool.BMP085_Pressure, 2);
  client.print("<br />");

  client.print("Runtime (ms): ");
  client.print(RunTime.bmp085);
  client.print("<br />");

  /* MS5611 sensor */
  client.print("<br />");
  client.print("MS5611 Sensor: ");
  client.print("<br />");
  
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
  
  client.println("</html>");
}
