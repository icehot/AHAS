// EEPROMUtil.h

#ifndef _EEPROMUTIL_h
#define _EEPROMUTIL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "EEPROM.h"

#define EEPROM_BASE 0x00
#define EEPROM_CONFIG_ADDRESS EEPROM_BASE
#define EEPROM_CONFIG_SIZE sizeof(eeprom_config)

#define EEPROM_PSWD_ADDRESS EEPROM_CONFIG_ADDRESS + EEPROM_CONFIG_SIZE
#define EEPROM_PSWD_SIZE 20

typedef struct config_t
{
    byte config_set;
    byte use_dhcp;
    byte dhcp_refresh_minutes;
    byte mac[6];
    byte ip[4];
    byte gateway[4];
    byte subnet[4];
    byte dns_server[4];
    unsigned int webserverPort;
} EEPROM_ConfigType;

extern EEPROM_ConfigType eeprom_config;

//template <class T> int EEPROM_writeAnything(int ee, const T& value);
//template <class T> int EEPROM_readAnything(int ee, T& value);

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}

#endif
