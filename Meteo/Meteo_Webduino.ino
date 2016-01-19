/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
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

  int i;   
  int val; 
  server << "{ ";
  val = digitalRead(2);
  server << "\"d" << 2 << "\": " << val << ", ";
  val = digitalRead(3);
  server << "\"d" << 3 << "\": " << val << ", ";
  val = digitalRead(5);
  server << "\"d" << 5 << "\": " << val << ", ";
  val = digitalRead(13);
  server << "\"d" << 13 << "\": " << val << ", ";
  
  /*
  for (i = 51; i <= 53; ++i)
  {
    // ignore the pins we use to talk to the Ethernet chip
    int val = digitalRead(i);
    server << "\"d" << i << "\": " << val << ", ";
  }
  */

  for (i = 0; i <= 5; ++i)
  {
    int val = analogRead(i);
    server << "\"a" << i << "\": " << val;
    if (i != 5)
      server << ", ";
  }
  
  server << " }";
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
      if (name[0] == 'd')
      {
        int pin = strtoul(name + 1, NULL, 10);
        int val = strtoul(value, NULL, 10);
        digitalWrite(pin, val);
      }
    } while (repeat);

    server.httpSeeOther(PREFIX "/");
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

