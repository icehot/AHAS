/** NTP - Network Time Protocol **/
#ifdef USE_NTP
#include <Time.h> 
#include <EthernetUdp.h>

/*Configuration*/

const int timeZone = 2;     // Eastern European Time
// NTP Servers:
IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov
unsigned int localPort = 8888;  // local port to listen for UDP packets
bool isInit = true;

EthernetUDP Udp;

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

void init_NTP()
{
  Udp.begin(localPort);
  
  #ifdef USE_SERIAL_MONITOR
    Serial.print("#INIT: Network Time Protocol =>");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: Network Time Protocol =>");
  #endif
  
  setSyncProvider(getNtpTime);
}

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

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets

    sendNTPpacket(timeServer);
  
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) 
  {
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

      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      
      unsigned long secsSince1900;
     
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
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

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
#endif
