/** EEPROM Memory **/

#include <EEPROM.h>

/***********************************************************************************************************/
/*** EEPROM Mapping ***/
/***********************************************************************************************************/
/* structure which is stored in the eeprom. 
* Look at "EEPROMAnything.h" for the functions storing and reading the struct
*/

#define EEPROM_BASE 0x00
#define EEPROM_CONFIG_ADDRESS EEPROM_BASE
#define EEPROM_CONFIG_SIZE sizeof(eeprom_config)

#define EEPROM_PSWD_ADDRESS EEPROM_CONFIG_ADDRESS + EEPROM_CONFIG_SIZE
#define EEPROM_PSWD_SIZE 20

struct config_t
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
} eeprom_config;

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
