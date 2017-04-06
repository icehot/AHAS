// Net.h

#ifndef _NET_h
#define _NET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef USE_ETH_SHIELD
extern unsigned long last_dhcp_renew;
extern byte dhcp_state;

typedef enum
{
    NothingHappened = 0,
    RenewFailed = 1,
    RenewSuccess = 2,
    RebindFailed = 3,
    RebindSuccess = 4
}enDHCP_State_Type;

void init_NetSetup();
void set_Default_Values();
void read_EEPROM_Settings();
void set_EEPROM_Default();
void renewDHCP(int interval);

#endif

#endif

