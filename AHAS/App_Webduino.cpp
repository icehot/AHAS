/** WebDuino - Webserver **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_EEPROM.h"
#include "App_Webduino.h"
#include "App_Net.h"
#include "SD.h"
#include "App_WebPages.h"


#ifdef USE_WEBDUINO

#define PREFIX ""

#define WEBDUINO_FAVICON_DATA ""

#define SIZE_OF_BUFFER 1024

#include <WebServer.h>

/* This creates an instance of the webserver.  By specifying a prefix
* of "", all pages will be at the root of the server. */

#define JSON_BEGIN() server << F("{ ");
#define JSON_ADD(name,value)  server << F("\"") << (name) << F("\": ") << (value) << F(", ");
#define JSON_ADD2(name,value)  server << F("\"") << (name) << F("\": ") << F("\"") << (value) << F("\"") << F(", ");
#define JSON_END() server << F(" \"end\": ") << 0 << F(" }");

/* This creates an pointer to instance of the webserver. */
WebServer * webserver;

// no-cost stream operator as described at 
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{
    obj.print(arg); return obj;
}

/* commands are functions that get called by the webserver framework
* they can read any posted data from client, and they output to the
* server to send data back to the web browser. */

#ifdef USE_SD

void sendHtmlFromSD(WebServer &server, char * filename)
{
    char HtmlReadBuffer[SIZE_OF_BUFFER + 1];
    unsigned long fileSize = 0;
    unsigned long quotient = 0;
    unsigned int remainder = 0;
    unsigned int i;

    /* Close the end of buffer with \0 */
    HtmlReadBuffer[SIZE_OF_BUFFER] = '\0';

    /* Open the file */
    File dataFile = SD.open(filename);

    /* If the file is available */
    if (dataFile)
    {
        /* Get the size */
        fileSize = dataFile.size();
        /* Calculate the nr of whole data chunks */
        quotient = fileSize / SIZE_OF_BUFFER;
        /* Calculate the remaining data chunk size */
        remainder = fileSize % SIZE_OF_BUFFER;

        /* Get the whole data chunks */
        for (i = 0; i < quotient; i++)
        {
            /* Read From File */
            dataFile.read(HtmlReadBuffer, SIZE_OF_BUFFER);
            /* Send to the browser */
            server << HtmlReadBuffer;
        }

        /* Get the remaining data chunk  */
        dataFile.read(HtmlReadBuffer, remainder);
        /* Close the end of buffer with \0 */
        HtmlReadBuffer[remainder] = '\0';
        /* Send to the browser */
        server << HtmlReadBuffer;

        /* Opent the file */
        dataFile.close();
    }
    else
    {/* if the file isn't open, pop up an error */
        MONITOR(F("#WEB: Error opening: "));
        MONITOR_LN(filename);
        LOG(F("#WEB: Error opening file"));
    }
}

#endif /* USE_SD */

void indexCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    /* this line sends the standard "we're all OK" headers back to the
    browser */
    server.httpSuccess();

    /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
    if (type != WebServer::HEAD)
    {
		#ifdef USE_SD
			sendHtmlFromSD(server, "index.htm");
		#else
			server << F(INDEX_HTM);
		#endif	
    }
}

void valuesCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{

    /* this line sends the standard "we're all OK" headers back to the
    browser */
    server.httpSuccess();

    /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
    if (type != WebServer::HEAD)
    {
		#ifdef USE_SD
			sendHtmlFromSD(server, "values.htm");
		#else
			server << F(VALUES_HTM);
		#endif	
    }
}

void graphsCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{

    /* this line sends the standard "we're all OK" headers back to the
    browser */
    server.httpSuccess();

    /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
    if (type != WebServer::HEAD)
    {
		#ifdef USE_SD
			sendHtmlFromSD(server, "graphs.htm");
		#else
			server << F(GRAPHS_HTM);
		#endif	
    }
}

void settingsCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
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

    char credentialsEE[30];
    EEPROM_readAnything(EEPROM_PSWD_ADDRESS, credentialsEE);

    if (server.checkCredentials(credentialsEE))
    {
        if (type == WebServer::POST)
        {
            bool repeat;
            char name[16], value[16];
            int index;
            do
            {
                repeat = server.readPOSTparam(name, 16, value, 16);

                index = strtoul(name + 1, NULL, 10);
                switch (name[0])
                {
                case 'm': /* mac address */
                    eeprom_config.mac[index] = strtol(value, NULL, 16);
                    break;

                case 'i': /* ip address */
                    eeprom_config.ip[index] = atoi(value);
                    break;

                case 's': /* subnet mask */
                    eeprom_config.subnet[index] = atoi(value);
                    break;

                case 'g': /* gateway address */
                    eeprom_config.gateway[index] = atoi(value);
                    break;

                case 'd': /* dns server */
                    eeprom_config.dns_server[index] = atoi(value);
                    break;

                case 'p': /* port */
                    eeprom_config.webserverPort = atoi(value);
                    break;

                case 'h': /* dhcp */
                    eeprom_config.use_dhcp = atoi(value);
                    break;

                case 'r': /* dhcp renew */
                    eeprom_config.dhcp_refresh_minutes = atoi(value);
                    break;

                default:
                    break;
                }

            } while (repeat);

            EEPROM_writeAnything(0, eeprom_config);

            server.httpSeeOther(PREFIX "/settings.htm");

            return;
        }

        server.httpSuccess();

        /* if we're handling a GET or POST, we can output our data here.
        For a HEAD request, we just stop after outputting headers. */
        if (type == WebServer::HEAD)
            return;

        if (type == WebServer::GET)
        {
			#ifdef USE_SD
				sendHtmlFromSD(server, "settings.htm");
			#else
				server << F(SETTINGS_HTM);
			#endif
        }
    }
    else
    {
        /* send a 401 error back causing the web browser to prompt the user for credentials */
        server.httpUnauthorized();
    }
}



void pswdChangeCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
    char credentialsEE[30];
    EEPROM_readAnything(EEPROM_PSWD_ADDRESS, credentialsEE);

    if (server.checkCredentials(credentialsEE))
    {
        if (type == WebServer::POST)
        {
            bool repeat;
            char name[16], value[30], temp[30];
            int index;
            do
            {
                repeat = server.readPOSTparam(name, 16, value, 30);

                if (strcmp(name, "pswd") == 0)
                {
                    EEPROM_writeAnything(EEPROM_PSWD_ADDRESS, value);
                    MONITOR(F("WEB: New Password saved to EEPROM:"));
                    EEPROM_readAnything(EEPROM_PSWD_ADDRESS, temp);
                    MONITOR_LN(temp);
                   LOG(F("WEB: New Password saved to EEPROM:"));
                }

            } while (repeat);

            server.httpSeeOther(PREFIX "/settings.htm");

            return;
        }

        server.httpSuccess();

        /* if we're handling a GET or POST, we can output our data here.
        For a HEAD request, we just stop after outputting headers. */
        if (type == WebServer::HEAD)
            return;
    }
}


#define USE_SYSTEM_LIBRARY //comment out if you want to save some space (about 1 Byte). You wouldn't see uptime and free RAM if it's commented out.

#ifdef USE_SYSTEM_LIBRARY
#include <System.h>
System sys;
#endif

void settingsJsonCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
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
    JSON_ADD(F("mac0"), eeprom_config.mac[0]);
    JSON_ADD(F("mac1"), eeprom_config.mac[1]);
    JSON_ADD(F("mac2"), eeprom_config.mac[2]);
    JSON_ADD(F("mac3"), eeprom_config.mac[3]);
    JSON_ADD(F("mac4"), eeprom_config.mac[4]);
    JSON_ADD(F("mac5"), eeprom_config.mac[5]);

    JSON_ADD(F("ip0"), eeprom_config.ip[0]);
    JSON_ADD(F("ip1"), eeprom_config.ip[1]);
    JSON_ADD(F("ip2"), eeprom_config.ip[2]);
    JSON_ADD(F("ip3"), eeprom_config.ip[3]);

    JSON_ADD(F("sub0"), eeprom_config.subnet[0]);
    JSON_ADD(F("sub1"), eeprom_config.subnet[1]);
    JSON_ADD(F("sub2"), eeprom_config.subnet[2]);
    JSON_ADD(F("sub3"), eeprom_config.subnet[3]);

    JSON_ADD(F("gw0"), eeprom_config.gateway[0]);
    JSON_ADD(F("gw1"), eeprom_config.gateway[1]);
    JSON_ADD(F("gw2"), eeprom_config.gateway[2]);
    JSON_ADD(F("gw3"), eeprom_config.gateway[3]);

    JSON_ADD(F("dns0"), eeprom_config.dns_server[0]);
    JSON_ADD(F("dns1"), eeprom_config.dns_server[1]);
    JSON_ADD(F("dns2"), eeprom_config.dns_server[2]);
    JSON_ADD(F("dns3"), eeprom_config.dns_server[3]);

    JSON_ADD(F("port"), eeprom_config.webserverPort);
    JSON_ADD(F("dhcp"), eeprom_config.use_dhcp);
    JSON_ADD(F("renew"), eeprom_config.dhcp_refresh_minutes);

    JSON_ADD(F("dhcp_state"), dhcp_state);
    JSON_ADD(F("dhcp_renew"), last_dhcp_renew / 1000);
    JSON_ADD2(F("uptime"), sys.uptime());
    JSON_ADD(F("ramsize"), sys.ramSize());
    JSON_ADD(F("ramfree"), sys.ramFree());

    JSON_END();
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
#ifdef USE_RELAY
    JSON_ADD(F("r1"), digitalRead(PIN_RELAY1));
    JSON_ADD(F("r2"), digitalRead(PIN_RELAY2));
    JSON_ADD(F("r3"), digitalRead(PIN_RELAY3));
    JSON_ADD(F("r4"), digitalRead(PIN_RELAY4));
#endif

    /* Analog inputs */
    JSON_ADD(F("a0"), analogRead(0));
    JSON_ADD(F("a1"), analogRead(PIN_MQ2));
    JSON_ADD(F("a2"), analogRead(PIN_WATERSENSOR));
    JSON_ADD(F("a3"), analogRead(PIN_LIGHTSENSOR));
    JSON_ADD(F("a4"), analogRead(PIN_ANALOG_BUTTON));

    /* DataPool variables */
#ifdef USE_DHT11
    JSON_ADD(F("dht11_temp"), DataPool.DHT11_Temperature);
    JSON_ADD(F("dht11_hum"), DataPool.DHT11_Humidity);
    JSON_ADD(F("dht11_dew"), DataPool.DHT11_DewPoint);
#endif

#ifdef USE_BMP085
    JSON_ADD(F("bmp085_temp"), DataPool.BMP085_Temperature);
    JSON_ADD(F("bmp085_pres"), DataPool.BMP085_Pressure);
#endif

#ifdef USE_MS5611
    JSON_ADD(F("ms5611_temp"), DataPool.MS5611_Temperature);
    JSON_ADD(F("ms5611_pres"), DataPool.MS5611_Pressure);
    JSON_ADD(F("ms5611_aalt"), DataPool.MS5611_AbsAltitude);
    JSON_ADD(F("ms5611_ralt"), DataPool.MS5611_RelAltitude);
#endif
#ifdef USE_DS1302
    JSON_ADD(F("ds1302_year"), DataPool.DS1302_Year + 2000);
    JSON_ADD(F("ds1302_month"), DataPool.DS1302_Month);
    JSON_ADD(F("ds1302_day"), DataPool.DS1302_Day);
    JSON_ADD(F("ds1302_hour"), DataPool.DS1302_Hour);
    JSON_ADD(F("ds1302_min"), DataPool.DS1302_Minute);
    JSON_ADD(F("ds1302_sec"), DataPool.DS1302_Second);
    JSON_ADD2(F("ds1302_sync"), DataPool.DS1302_SyncStatus);
#endif

#ifdef USE_PIR
    JSON_ADD(F("pir_state"), DataPool.PIR_State);
#endif 
#ifdef USE_SOUND_DETECT
    JSON_ADD(F("sound_state"), DataPool.SOUND_State);
#endif

#ifdef USE_RGB
    JSON_ADD(F("rgb_red"), DataPool.RGB_Red);
    JSON_ADD(F("rgb_green"), DataPool.RGB_Green);
    JSON_ADD(F("rgb_blue"), DataPool.RGB_Blue);
#endif

#ifdef USE_NTP
    JSON_ADD(F("ntp_year"), DataPool.NTP_Year);
    JSON_ADD(F("ntp_month"), DataPool.NTP_Month);
    JSON_ADD(F("ntp_day"), DataPool.NTP_Day);
    JSON_ADD(F("ntp_hour"), DataPool.NTP_Hour);
    JSON_ADD(F("ntp_min"), DataPool.NTP_Minute);
    JSON_ADD(F("ntp_sec"), DataPool.NTP_Second);
    JSON_ADD2(F("ntp_sync"), DataPool.NTP_Status);
#endif

    JSON_END();
}
#ifdef USE_RELAY
void inputCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
    if (type == WebServer::POST)
    {
        bool repeat;
        char name[16], value[16];
        do
        {
            repeat = server.readPOSTparam(name, 16, value, 16);

            if (strcmp(name, "mac1") == 0)
            {

            }

            if (name[0] == 'r')
            {
                int relay = strtoul(name + 1, NULL, 10);
                int val = strtoul(value, NULL, 10);

                switch (relay)
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

    /* for a GET or HEAD, send the standard "it's all OK headers" */
    server.httpSuccess();
}
#endif

#ifdef USE_RGB
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
#endif
/**
* errorHTML() function
* This function is called whenever a non extisting page is called.
* It sends a HTTP 400 Bad Request header and the same as text.
*/
void errorHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
    /* this line sends the standard "HTTP 400 Bad Request" headers back to the
    browser */
    server.httpFail();

    /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
    if (type == WebServer::HEAD)
        return;

    //server.printP("HTTP 400 - BAD REQUEST");
    server << F("HTTP 400 - BAD REQUEST");

    //server.printP("</body></html>");
    server << F("</body></html>");
}

void noSDCardCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
    /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
    if (type == WebServer::HEAD)
        return;

    server << F("<body><html>");
    server << F("<h1> Arduino Home Automation System </h1>");

    server << F("<p>No SD card support is enabled</p>");

    server << F("</body></html>");
}

void init_Webduino()
{
    webserver = new WebServer(PREFIX, eeprom_config.webserverPort);

    /* setup our default command that will be run when the user accesses
    * the root page on the server */

    webserver->setDefaultCommand(&indexCmd);
    webserver->addCommand("index.htm", &indexCmd);
    webserver->addCommand("values.htm", &valuesCmd);
    webserver->addCommand("settings.htm", &settingsCmd);
    webserver->addCommand("graphs.htm", &graphsCmd);
    webserver->setFailureCommand(&errorHTML);

#ifdef USE_RELAY
    webserver->addCommand("input", &inputCmd);
#endif

#ifdef USE_RGB
    webserver->addCommand("rgb", &rgbCmd);
#endif

    webserver->addCommand("json", &jsonCmd);
    webserver->addCommand("settingsJSON", &settingsJsonCmd);
    webserver->addCommand("pswdChange", &pswdChangeCmd);


    /* start the webserver */
    webserver->begin();

    MONITOR_LOG_LN(F("#INIT: WebDuino Server => DONE"));
}

void WebduinoServerLoop()
{
    char buff[SIZE_OF_BUFFER];
    int len = SIZE_OF_BUFFER;

    /* process incoming connections one at a time forever */
    webserver->processConnection(buff, &len);
}
#endif


