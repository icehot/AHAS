/** NTP - Network Time Protocol **/
#ifdef USE_NTP
#include <Time.h> 
#include <EthernetUdp.h>

/* Configuration */

/* NTP Servers: */
#define NR_OF_TIMESERVERS 3
IPAddress timeServerA(132, 163, 4, 101); /* time-a.timefreq.bldrdoc.gov */
IPAddress timeServerB(132, 163, 4, 102); /* time-b.timefreq.bldrdoc.gov */
IPAddress timeServerC(132, 163, 4, 103); /* time-c.timefreq.bldrdoc.gov */

IPAddress * timeServer[NR_OF_TIMESERVERS];

/* Local port to listen for UDP packets */
unsigned int localPort = 8888;
  
/* Eastern European Time */
const byte timeZone = 2;  

const byte dayLightSaving = 1;

/* Flag that indicates the initialization state*/
bool isInit = true;

/* Handle to UDP connection */ 
EthernetUDP Udp;

/*
 * Function Declarations
 */
time_t getNtpTime();
time_t getNtpTimeMultiServer();
void sendNTPpacket(IPAddress &address);
time_t getNtpResponse();

/*
 * Initialize the NTP
 */
void init_NTP()
{
  Udp.begin(localPort);

  timeServer[0] = &timeServerA;
  timeServer[1] = &timeServerB;
  timeServer[2] = &timeServerC;
    
  #ifdef USE_SERIAL_MONITOR
    Serial.print("#INIT: Network Time Protocol =>");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: Network Time Protocol =>");
  #endif
  
  setSyncProvider(getNtpTimeMultiServer);
}

/*
 * Function to get the time
 */
void read_time()
{
  time_t t;
  t = now();

  DataPool.NTP_Year = year(t);
  DataPool.NTP_Month = month(t);
  DataPool.NTP_Day = day(t);
  DataPool.NTP_Hour = hour(t);
  DataPool.NTP_Minute = minute(t);
  DataPool.NTP_Second = second(t);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

/*
 * Function to get the NTP time
 */
time_t getNtpTime()
{
   /* discard any previously received packets */
  while (Udp.parsePacket() > 0) ;

  /* Send NTP Packet to the timeserver */
  sendNTPpacket(timeServerA);

  /* Wait for NTTP response, timeout is 1500 ma */
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) 
  {
    /* Poll for an NTP response */
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) 
    {
      #ifdef USE_SERIAL_MONITOR
      if (isInit == true)
      {
        Serial.println("#DONE");
        isInit = false;
      }
      else
      {
        Serial.println("#NTP: Response Received");
      }
      #endif
      #ifdef USE_SYS_LOG
        add2SysLog("#NTP: Response Received");
      #endif

      /* Read packet into the buffer */
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  
      
      unsigned long secsSince1900;
     
      /* Convert four bytes starting at location 40 to a long integer */
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      
      /* Return the time since 1970  + timezone correction */
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  
  #ifdef USE_SERIAL_MONITOR
    Serial.println("Failed");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#NTP: Response Failed");
  #endif
    
  return 0; // return 0 if unable to get the time
}

/*
 * Function to get the NTP time
 */
time_t getNtpTimeMultiServer()
{
  unsigned long secsSince1900 = 0;
  byte i;
  
   /* discard any previously received packets */
  while (Udp.parsePacket() > 0) ;

  /* Send NTP Packet to the timeservers */

  for (i=0; i < NR_OF_TIMESERVERS; i++ )
  {
    sendNTPpacket(*timeServer[i]);
    secsSince1900 = getNtpResponse();

    if (secsSince1900 != 0)
    {
      #ifdef USE_SERIAL_MONITOR
      if (isInit == true)
      {
        Serial.println("#DONE");
        Serial.print("#NTP: Response Received from server #");
        Serial.println(i+1);
        isInit = false;
      }
      else
      {
        Serial.print("#NTP: Response Received from server #");
        Serial.println(i+1);
      }
      #endif
      #ifdef USE_SYS_LOG
        add2SysLog("#NTP: Response Received");
      #endif
      break;
    }
  }

  if ((i == NR_OF_TIMESERVERS) && (secsSince1900 == 0))
  {
     #ifdef USE_SERIAL_MONITOR
      Serial.println("FAILED");
    #endif
    #ifdef USE_SYS_LOG
      add2SysLog("#NTP: Response Failed");
    #endif
    return 0; // return 0 if unable to get the time
  }

  /* Return the time since 1970 + time zone correction */
  return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR + dayLightSaving*SECS_PER_HOUR;
}


/*
 * Send an NTP request to the time server at the given address
 */
void sendNTPpacket(IPAddress &address)
{
  /* set all bytes in the buffer to 0 */
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  
  /* Initialize values needed to form NTP request */
  packetBuffer[0] = 0b11100011;   /* LI, Version, Mode */
  packetBuffer[1] = 0;            /* Stratum, or type of clock */
  packetBuffer[2] = 6;            /* Polling Interval */
  packetBuffer[3] = 0xEC;         /*  Peer Clock Precision */
  /* 8 bytes of zero for Root Delay & Root Dispersion */
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  /* Send a packet requesting a timestamp:  */               
  Udp.beginPacket(address, 123); /* NTP requests are to port 123 */
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
/*
 * Get the NTP response
 */
time_t getNtpResponse()
{
  unsigned long secsSince1900;
  
  /* Wait for NTTP response, timeout is 1500 ma */
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) 
  {
    /* Poll for an NTP response */
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) 
    {
      /* Read packet into the buffer */
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  
      
      /* Convert four bytes starting at location 40 to a long integer */
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      
      /* Return the time since 1900 */
      return secsSince1900;
    }
  }
  
  /* return 0 if unable to get the time */
  return 0; 
}
#endif
