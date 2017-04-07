/** NTP - Network Time Protocol **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"


#ifdef USE_NTP
#include <TimeX.h> 
#include <EthernetUdp.h>

#include "App_NTP.h"

/* Configuration */

/* NTP Servers: */
#define NR_OF_TIMESERVERS 3
IPAddress timeServerA(132, 163, 4, 101); /* time-a.timefreq.bldrdoc.gov */
IPAddress timeServerB(132, 163, 4, 102); /* time-b.timefreq.bldrdoc.gov */
IPAddress timeServerC(132, 163, 4, 103); /* time-c.timefreq.bldrdoc.gov */

IPAddress * timeServer[NR_OF_TIMESERVERS];

/* Local port to listen for UDP packets */
unsigned int localPort = 8888;

/* Time Zone correction */
const byte timeZone = 2; /* Eastern European Time */

const byte dayLightSaving = 1;

/* Flag that indicates the initialization state*/
bool isInit = true;

/* Handle to UDP connection */
EthernetUDP Udp;


/*
* Initialize the NTP
*/
void init_NTP()
{
    Udp.begin(localPort);

    timeServer[0] = &timeServerA;
    timeServer[1] = &timeServerB;
    timeServer[2] = &timeServerC;

    MONITOR_LOG(F("#INIT: Network Time Protocol => "));

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
    while (Udp.parsePacket() > 0);

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
            if (isInit == true)
            {
                MONITOR_LOG_LN(F("#DONE"));
                isInit = false;
            }
            else
            {
                MONITOR_LOG_LN(F("#NTP: Response Received"));
            }

            /* Read packet into the buffer */
            Udp.read(packetBuffer, NTP_PACKET_SIZE);

            unsigned long secsSince1900;

            /* Convert four bytes starting at location 40 to a long integer */
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];

            /* Return the time since 1970  + timezone correction */
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }

    MONITOR_LOG_LN(F("Failed"))

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
    while (Udp.parsePacket() > 0);

    /* Send NTP Packet to the timeservers */

    for (i = 0; i < NR_OF_TIMESERVERS; i++)
    {
        sendNTPpacket(*timeServer[i]);
        secsSince1900 = getNtpResponse();

        if (secsSince1900 != 0)
        {
            if (isInit == true)
            {
                MONITOR_LN(F("#DONE"));
                MONITOR(F("#NTP: Response Received from server #"));
                MONITOR_LN(i + 1);
                isInit = false;
            }
            else
            {
                MONITOR(F("#NTP: Response Received from server #"));
                MONITOR_LN(i + 1);
            }

            LOG(F("#NTP: Response Received"));
            break;
        }
    }

    if ((i == NR_OF_TIMESERVERS) && (secsSince1900 == 0))
    {
        MONITOR_LN(F("FAILED"));
        LOG(F("#NTP: Response Failed"));
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
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

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
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
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

