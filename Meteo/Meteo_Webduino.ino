/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */

#define JSON_BEGIN() server << "{ ";
#define JSON_ADD(name,value)  server << "\"" << (name) << "\": " << (value) << ", ";
#define JSON_END() server << " \"end\": " << 0 << " }";

WebServer webserver(PREFIX, PORT);

// no-cost stream operator as described at 
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void indexCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */

     // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("index.htm");
  
    // if the file is available, write to it:
    if (dataFile) 
    {
      while (dataFile.available()) 
      {
        server << ((char)(dataFile.read()));
      }
      dataFile.close();
    }
    else 
    {/* if the file isn't open, pop up an error */
      Serial.println("Error opening index.htm");
    }
  }
}

void settingsCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* The credentials have to be concatenated with a colon like
   * username:password
   * and encoded using Base64 - this should be done outside of your Arduino
   * to be easy on your resources
   *
   * in other words: "dXNlcjp1c2Vy" is the Base64 representation of "user:user"
   *
   * if you need to change the username/password dynamically please search
   * the web for a Base64 library */
   
  /* if the user has requested this page using the following credentials
   * username = admin
   * password = admin
   * display a page saying "Hello Admin"
   *
   * in other words: "YWRtaW46YWRtaW4=" is the Base64 representation of "admin:admin" */
  if (server.checkCredentials("YWRtaW46YWRtaW4="))
  {
      server.httpSuccess();
      if (type != WebServer::HEAD)
      {
         /* this defines some HTML text in read-only memory aka PROGMEM.
         * This is needed to avoid having the string copied to our limited
         * amount of RAM. */
    
         // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open("settings.htm");
      
        // if the file is available, write to it:
        if (dataFile) 
        {
          while (dataFile.available()) 
          {
            server << ((char)(dataFile.read()));
          }
          dataFile.close();
        }
        else 
        {/* if the file isn't open, pop up an error */
          Serial.println("Error opening settings.htm");
        }
      }
  }
  else
  {
    /* send a 401 error back causing the web browser to prompt the user for credentials */
    server.httpUnauthorized();
  }
}

void jsonCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (type == WebServer::POST)
  {
    server.httpFail();
    return;
  }
  
  //server.httpSuccess(false, "application/json");
  server.httpSuccess("application/json");
  
  if (type == WebServer::HEAD)
    return;
  
  
  JSON_BEGIN();
  
  /* Relays */
  JSON_ADD("r1",digitalRead(PIN_RELAY1));
  JSON_ADD("r2",digitalRead(PIN_RELAY2));
  JSON_ADD("r3",digitalRead(PIN_RELAY3));
  JSON_ADD("r4",digitalRead(PIN_RELAY4));
  
  /* Analog inputs */
  JSON_ADD("a0",analogRead(0));
  JSON_ADD("a1",analogRead(1));
  JSON_ADD("a2",analogRead(2));
  JSON_ADD("a3",analogRead(3));
  JSON_ADD("a4",analogRead(4));

  /* DataPool variables */
  JSON_ADD("dht11_temp",DataPool.DHT11_Temperature);
  JSON_ADD("dht11_hum",DataPool.DHT11_Humidity);
  JSON_ADD("dht11_dew",DataPool.DHT11_DewPoint);
  JSON_ADD("bmp085_temp",DataPool.BMP085_Temperature);
  JSON_ADD("bmp085_pres",DataPool.BMP085_Pressure);
  JSON_ADD("ms5611_temp",DataPool.MS5611_Temperature);
  JSON_ADD("ms5611_pres",DataPool.MS5611_Pressure);
  JSON_ADD("ms5611_aalt",DataPool.MS5611_AbsAltitude);
  JSON_ADD("ms5611_ralt",DataPool.MS5611_RelAltitude);
  JSON_ADD("ds1302_year",DataPool.DS1302_Year);
  JSON_ADD("ds1302_month",DataPool.DS1302_Month);
  JSON_ADD("ds1302_day",DataPool.DS1302_Day);
  JSON_ADD("ds1302_hour",DataPool.DS1302_Hour);
  JSON_ADD("ds1302_min",DataPool.DS1302_Minute);
  JSON_ADD("ds1302_sec",DataPool.DS1302_Second);
    
  JSON_END();
}

void inputCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (type == WebServer::POST)
  {
    bool repeat;
    char name[16], value[16];
    do
    {
      repeat = server.readPOSTparam(name, 16, value, 16);
      
      if (name[0] == 'r')
      {
        int relay = strtoul(name + 1, NULL, 10);
        int val = strtoul(value, NULL, 10);

        switch(relay)
        {
            case 1: 
              digitalWrite(PIN_RELAY1, val);
            break;
            
            case 2: 
              digitalWrite(PIN_RELAY2, val);
            break;
            
            case 3: 
              digitalWrite(PIN_RELAY3, val);
            break;
            
            case 4: 
              digitalWrite(PIN_RELAY4, val);
            break;

            default:
              /*Unrecognized relay number*/
            break;
        }
      }
    } while (repeat);

    server.httpSeeOther(PREFIX "/");
  }
}

void rgbCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  if (type == WebServer::POST)
  {
    bool repeat;
    char name[16], value[16];
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);

      /* this is a standard string comparison function.  It returns 0
       * when there's an exact match.  We're looking for a parameter
       * named red/green/blue here. */
      if (strcmp(name, "red") == 0)
      {
  /* use the STRing TO Unsigned Long function to turn the string
   * version of the color strength value into our integer red/green/blue
   * variable */
        DataPool.RGB_Red = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "green") == 0)
      {
        DataPool.RGB_Green = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "blue") == 0)
      {
        DataPool.RGB_Blue = strtoul(value, NULL, 10);
      }
    } while (repeat);
    
    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(PREFIX);

    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();
}

void init_Webduino()
{
  //Ethernet connection and server initialization
   Ethernet.begin(mac); 
   /* setup our default command that will be run when the user accesses
   * the root page on the server */
  webserver.setDefaultCommand(&indexCmd);

  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &indexCmd);
  webserver.addCommand("settings.htm", &settingsCmd);
  webserver.addCommand("input", &inputCmd);
  webserver.addCommand("json", &jsonCmd);
  webserver.addCommand("rgb",&rgbCmd);
  
  /* start the webserver */
  webserver.begin();
}

void WebduinoServerLoop()
{
  char buff[64];
  int len = 64;

  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
}

