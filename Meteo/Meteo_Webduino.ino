/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""

#define JSON_BEGIN() server << "{ ";
#define JSON_ADD(name,value)  server << "\"" << (name) << "\": " << (value) << ", ";
#define JSON_END() server << " \"end\": " << 0 << " }";


byte mac[] = { 0xBE, 0xD0, 0xBE, 0xD0, 0xBE, 0xD0 };

WebServer webserver(PREFIX, 89);

// no-cost stream operator as described at 
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
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
        red = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "green") == 0)
      {
        green = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "blue") == 0)
      {
        blue = strtoul(value, NULL, 10);
      }
    } while (repeat);
    
    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(PREFIX);

    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
    /* store the HTML in program memory using the P macro */
    P(message) = 
"<!DOCTYPE html><html><head>"
  "<title>Webduino AJAX RGB Example</title>"
  "<link href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/themes/base/jquery-ui.css' rel=stylesheet />"
  "<script src='http://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js'></script>"
  "<script src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/jquery-ui.min.js'></script>"
  "<style> body { background: black; } #red, #green, #blue { margin: 10px; } #red { background: #f00; } #green { background: #0f0; } #blue { background: #00f; } </style>"
  "<script>"

// change color on mouse up, not while sliding (causes much less traffic to the Arduino):
    "function changeRGB(event, ui) { var id = $(this).attr('id'); if (id == 'red') $.post('/rgb', { red: ui.value } ); if (id == 'green') $.post('/rgb', { green: ui.value } ); if (id == 'blue') $.post('/rgb', { blue: ui.value } ); } "
    "$(document).ready(function(){ $('#red, #green, #blue').slider({min: 0, max:255, change:changeRGB}); });"

// change color on slide and mouse up (causes more traffic to the Arduino):
//    "function changeRGB(event, ui) { jQuery.ajaxSetup({timeout: 110}); /*not to DDoS the Arduino, you might have to change this to some threshold value that fits your setup*/ var id = $(this).attr('id'); if (id == 'red') $.post('/rgb', { red: ui.value } ); if (id == 'green') $.post('/rgb', { green: ui.value } ); if (id == 'blue') $.post('/rgb', { blue: ui.value } ); } "
//    "$(document).ready(function(){ $('#red, #green, #blue').slider({min: 0, max:255, change:changeRGB, slide:changeRGB}); });"

  "</script>"
"</head>"
"<body style='font-size:62.5%;'>"
  "<div id=red></div>"
  "<div id=green></div>"
  "<div id=blue></div>"
"</body>"
"</html>";

    server.printP(message);
  }
}

void init_Webduino()
{
  //Ethernet connection and server initialization
   Ethernet.begin(mac); 
   /* setup our default command that will be run when the user accesses
   * the root page on the server */
  webserver.setDefaultCommand(&helloCmd);

  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &helloCmd);
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

