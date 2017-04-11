/** Net Setup **/
/** Ethernet Shield **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_Net.h"
#include "App_EEPROM.h"
#include "App_OS.h"

#ifdef USE_ETH_SHIELD

#include <SPI.h>
#include <Ethernet.h>

unsigned long last_dhcp_renew;
byte dhcp_state;


/**********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/


/**
* init_NetSetup() function
* This function is used to setup the network according to the values stored in the eeprom
*
* Overview:
* - First of all read the EEPROM settings
* - Display a link to the ethernet setup
* - Check if DHCP should be used, if not create instaces of IPAddress for ip, gateway, subnet and dns_server
* - Invoke Ethernet.begin with all parameters if no dhcp is active (Ethernet.begin(mac, ip, dns_server, gateway, subnet);).
* - If DHCP is used invoke only with mac (Ethernet.begin(mac);) and display the ip on the serial console.
*/

void init_NetSetup()
{
    bool dhcp_success = false;
    read_EEPROM_Settings();

#ifdef DEBUG_NET
    print_EEPROM_Settings();
#endif

    if (eeprom_config.use_dhcp == 1)
    {
        MONITOR_LOG(F("#INIT: Network via DHCP => "));

        if (Ethernet.begin(eeprom_config.mac) == 1)
        {
            dhcp_success = true;

            MONITOR_LOG_LN(F(" DONE"));
            MONITOR_LOG(F(" Obtained IP Address through DHCP: "));
            MONITOR_LN(Ethernet.localIP()); 
            //@Todo: IP address not saved to LOG 

            Task_RenewDHCP.setInterval(TASK_MINUTE*eeprom_config.dhcp_refresh_minutes);
            Task_RenewDHCP.enableDelayed(TASK_HOUR);
        }
        else
        {
            dhcp_success = false;
            MONITOR_LOG_LN(F(" FAILED"));
        }
    }

    if ((eeprom_config.use_dhcp == 0) || (dhcp_success == false))
    {
        IPAddress ip(eeprom_config.ip[0], eeprom_config.ip[1], eeprom_config.ip[2], eeprom_config.ip[3]);
        IPAddress gateway(eeprom_config.gateway[0], eeprom_config.gateway[1], eeprom_config.gateway[2], eeprom_config.gateway[3]);
        IPAddress subnet(eeprom_config.subnet[0], eeprom_config.subnet[1], eeprom_config.subnet[2], eeprom_config.subnet[3]);
        IPAddress dns_server(eeprom_config.dns_server[0], eeprom_config.dns_server[1], eeprom_config.dns_server[2], eeprom_config.dns_server[3]);
        Ethernet.begin(eeprom_config.mac, ip, dns_server, gateway, subnet);

        MONITOR_LOG_LN(F("#INIT: Network with default settings => DONE"));
        MONITOR_LOG(F(" DefaultIP Address : "));
        MONITOR_LN(Ethernet.localIP());
        //@Todo: IP address not saved to LOG 
    }
}

void set_Default_Values()
{
    char credentialsEE[20];
    char admin[20] = { 'Y', 'W', 'R', 't', 'a', 'W', '4', '6', 'Y', 'W', 'R', 't', 'a', 'W', '4', '=', '\0' };

    EEPROM_writeAnything(EEPROM_PSWD_ADDRESS, admin);//admin:admin

    // set default values
    set_EEPROM_Default();

    // write the config to eeprom
    EEPROM_writeAnything(0, eeprom_config);
}

/**
* read_EEPROM_Settings function
* This function is used to read the EEPROM settings at startup
*
* Overview:
* - Set the PIN for the RESET-button to input and activate pullups
* - Load the stored data from EEPROM into the eeprom_config struct
* - Check if a config is stored or the reset button is pressed. If one of the conditions is ture, set the defaults
*/

void read_EEPROM_Settings()
{
    /* Read the current config */
    EEPROM_readAnything(0, eeprom_config);

    /* Check if config is present */
    if ((eeprom_config.config_set != 1))
    {
        set_Default_Values();
    }

    #ifdef USE_ANALOG_BTN
    /* Check if reset button is pressed */
    if (analogRead(PIN_ANALOG_BUTTON) < UL_BTE)
    {
        set_Default_Values();
    }
    #endif
}

/**
* set_EEPROM_Default() function
*
* The default settings.
* This settings are used when no config is present or the reset button is pressed.
*/
void set_EEPROM_Default()
{
    eeprom_config.config_set = 1;  // dont change! It's used to check if the config is already set

    eeprom_config.use_dhcp = 1; // use DHCP per default
    eeprom_config.dhcp_refresh_minutes = 60; // refresh the DHCP every 60 minutes

    // set the default MAC address. In this case its BE:D0:BE:D0:BE:D0
    eeprom_config.mac[0] = 0xBE;
    eeprom_config.mac[1] = 0xD0;
    eeprom_config.mac[2] = 0xBE;
    eeprom_config.mac[3] = 0xD0;
    eeprom_config.mac[4] = 0xBE;
    eeprom_config.mac[5] = 0xD0;

    // set the default IP address for the arduino. In this case its 192.168.0.104
    eeprom_config.ip[0] = 192;
    eeprom_config.ip[1] = 168;
    eeprom_config.ip[2] = 0;
    eeprom_config.ip[3] = 104;

    // set the default GATEWAY. In this case its 192.168.0.1
    eeprom_config.gateway[0] = 192;
    eeprom_config.gateway[1] = 168;
    eeprom_config.gateway[2] = 0;
    eeprom_config.gateway[3] = 1;

    // set the default SUBNET. In this case its 255.255.255.0
    eeprom_config.subnet[0] = 255;
    eeprom_config.subnet[1] = 255;
    eeprom_config.subnet[2] = 255;
    eeprom_config.subnet[3] = 0;

    // set the default DNS SERVER. In this case its 192.168.0.254
    eeprom_config.dns_server[0] = 192;
    eeprom_config.dns_server[1] = 168;
    eeprom_config.dns_server[2] = 0;
    eeprom_config.dns_server[3] = 254;

    // set the default Webserver Port. In this case its Port 80
    eeprom_config.webserverPort = 80;

    MONITOR_LOG_LN(F("#NET: Default configuration restored"));
}

/**
* print_EEPROM_Settings() function
*
* This function is used for debugging the configuration.
* It prints the actual configuration to the serial port.
*/
#ifdef DEBUG_NET
void print_EEPROM_Settings() {
    Serial.print(F("IP: "));
    for (int i = 0; i<4; i++) {
        Serial.print(eeprom_config.ip[i]);
        if (i<3) {
            Serial.print('.');
        }
    }
    Serial.println();

    Serial.print(F("Subnet: "));
    for (int i = 0; i<4; i++) {
        Serial.print(eeprom_config.subnet[i]);
        if (i<3) {
            Serial.print('.');
        }
    }
    Serial.println();

    Serial.print(F("Gateway: "));
    for (int i = 0; i<4; i++) {
        Serial.print(eeprom_config.gateway[i]);
        if (i<3) {
            Serial.print('.');
        }
    }
    Serial.println();

    Serial.print(F("DNS Server: "));
    for (int i = 0; i<4; i++) {
        Serial.print(eeprom_config.dns_server[i]);
        if (i<3) {
            Serial.print('.');
        }
    }
    Serial.println();

    Serial.print(F("MAC: "));
    for (int a = 0; a<6; a++) {
        Serial.print(eeprom_config.mac[a], HEX);
        if (a<5) {
            Serial.print(":");
        }
    }
    Serial.println();

    Serial.print(F("Webserver Port: "));
    Serial.println(eeprom_config.webserverPort);

    Serial.print(F("USE DHCP: "));
    Serial.println(eeprom_config.use_dhcp);

    Serial.print(F(" DHCP renew every "));
    Serial.print(eeprom_config.dhcp_refresh_minutes);
    Serial.println(F(" minutes"));

    Serial.print(F("Config Set: "));
    Serial.println(eeprom_config.config_set);
}
#endif

/**
* renewDHCP() function
* Renew the DHCP relase in a given interval.
*
* Overview:
* - Check if interval = 0 and set it to 1
* - Check if renew interval is reached and renew the lease
*/
void renewDHCP(int interval) {
    unsigned long interval_millis = interval * 60000;

    if (interval == 0)
    {
        interval = 1;
    }

    if (eeprom_config.use_dhcp == 1)
    {
        if ((millis() - last_dhcp_renew) >  interval_millis) {
            last_dhcp_renew = millis();
            dhcp_state = Ethernet.maintain();
        }
    }
}


#endif

