// NTP.h

#ifndef _NTP_h
#define _NTP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TimeX.h"
#include "Ethernet.h"

/*
* Function Declarations
*/
void init_NTP();
void read_time();
time_t getNtpTime();
time_t getNtpTimeMultiServer();
void sendNTPpacket(IPAddress &address);
time_t getNtpResponse();

#endif

