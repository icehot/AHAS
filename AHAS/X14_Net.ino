/** Net Setup **/
/** Ethernet Shield **/
#ifdef USE_ETH_SHIELD
//#define DEBUG

#include <SPI.h>
#include <Ethernet.h>

unsigned long last_dhcp_renew;
byte dhcp_state;

enum
{
  NothingHappened = 0,
  RenewFailed     = 1,
  RenewSuccess    = 2,
  RebindFailed    = 3,
  RebindSuccess   = 4
}enDHCP_State;

/***********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/
void read_EEPROM_Settings();
void print_EEPROM_Settings();
void set_EEPROM_Default();
template <class T> int EEPROM_writeAnything(int ee, const T& value);
template <class T> int EEPROM_readAnything(int ee, T& value);


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
  
  #ifdef DEBUG
   print_EEPROM_Settings();
  #endif

  if (eeprom_config.use_dhcp == 1)
  { 
    
    
    if (Ethernet.begin(eeprom_config.mac) == 1)
    {
      dhcp_success = true;

      #ifdef USE_SERIAL_MONITOR
        Serial.println("#INIT: Network via DHCP => DONE");
        Serial.print(" Obtained IP Address through DHCP: ");
        Serial.println(Ethernet.localIP());
      #endif
      #ifdef USE_SYS_LOG
        add2SysLog("#INIT: Network via DHCP => DONE");
        add2SysLog(" Obtained IP Address through DHCP: ");
        //add2SysLog(Ethernet.localIP());
      #endif
    } 
    else
    {
      dhcp_success = false;
      #ifdef USE_SERIAL_MONITOR
        Serial.print("#INIT: Network via DHCP => FAILED");
      #endif
      #ifdef USE_SYS_LOG
        add2SysLog("#INIT: Network via DHCP => FAILED");
      #endif
    }
  }
  
  if ((eeprom_config.use_dhcp == 0) || (dhcp_success == false))  
  {
    IPAddress ip(eeprom_config.ip[0], eeprom_config.ip[1], eeprom_config.ip[2], eeprom_config.ip[3]);                                               
    IPAddress gateway (eeprom_config.gateway[0],eeprom_config.gateway[1],eeprom_config.gateway[2],eeprom_config.gateway[3]);                      
    IPAddress subnet  (eeprom_config.subnet[0], eeprom_config.subnet[1], eeprom_config.subnet[2], eeprom_config.subnet[3]);  
    IPAddress dns_server  (eeprom_config.dns_server[0], eeprom_config.dns_server[1], eeprom_config.dns_server[2], eeprom_config.dns_server[3]);
    Ethernet.begin(eeprom_config.mac, ip, dns_server, gateway, subnet);
    
    #ifdef USE_SERIAL_MONITOR
     Serial.println("#INIT: Network with default settings => DONE");
     Serial.print(" DefaultIP Address : ");
     Serial.println(Ethernet.localIP());
    #endif
    #ifdef USE_SYS_LOG
      add2SysLog("#INIT: Network with default settings => DONE");
      add2SysLog(" Obtained IP Address through DHCP: ");
      //add2SysLog(Ethernet.localIP());
    #endif
  } 

  if (eeprom_config.use_dhcp==1) 
  {
    Task_RenewDHCP.setInterval(TASK_MINUTE*eeprom_config.dhcp_refresh_minutes);
    Task_RenewDHCP.enable();
  }
}

void set_Default_Values()
{
    char credentialsEE[20];
    char admin[20]={'Y','W','R','t','a','W','4','6','Y','W','R','t','a','W','4','=','\0'};
  
    EEPROM_writeAnything(EEPROM_PSWD_ADDRESS, admin);//admin:admin
    
    // set default values
    set_EEPROM_Default();
    
    #ifdef USE_DEBUG_LED
    digitalWrite(PIN_DEBUG_LED,HIGH);
    #endif
       
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
  if ((eeprom_config.config_set != 1 ))
  {
    set_Default_Values();
  } 

  #ifdef USE_FACTDEF_BTN
  /* Check if reset button is pressed */
  digitalWrite(PIN_RESET, HIGH);
  if (digitalRead(PIN_RESET) == LOW)
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
    eeprom_config.config_set=1;  // dont change! It's used to check if the config is already set
  
    eeprom_config.use_dhcp=1; // use DHCP per default
    eeprom_config.dhcp_refresh_minutes=60; // refresh the DHCP every 60 minutes
  
    // set the default MAC address. In this case its BE:D0:BE:D0:BE:D0
    eeprom_config.mac[0]=0xBE;  
    eeprom_config.mac[1]=0xD0;
    eeprom_config.mac[2]=0xBE;
    eeprom_config.mac[3]=0xD0;
    eeprom_config.mac[4]=0xBE;
    eeprom_config.mac[5]=0xD0;
    
    // set the default IP address for the arduino. In this case its 192.168.0.104
    eeprom_config.ip[0]=192;
    eeprom_config.ip[1]=168;
    eeprom_config.ip[2]=0;
    eeprom_config.ip[3]=104;
  
    // set the default GATEWAY. In this case its 192.168.0.1
    eeprom_config.gateway[0]=192;
    eeprom_config.gateway[1]=168;
    eeprom_config.gateway[2]=0;
    eeprom_config.gateway[3]=1;
    
    // set the default SUBNET. In this case its 255.255.255.0
    eeprom_config.subnet[0]=255;
    eeprom_config.subnet[1]=255;
    eeprom_config.subnet[2]=255;
    eeprom_config.subnet[3]=0;

    // set the default DNS SERVER. In this case its 192.168.0.254
    eeprom_config.dns_server[0]=192;
    eeprom_config.dns_server[1]=168;
    eeprom_config.dns_server[2]=0;
    eeprom_config.dns_server[3]=254;

    // set the default Webserver Port. In this case its Port 89
    eeprom_config.webserverPort=89;
    
    #ifdef DEBUG
      Serial.println("#NET: Config reset");
    #endif 
    #ifdef USE_SYS_LOG
      add2SysLog("#NET: Config reset");
    #endif
}

/**
* print_EEPROM_Settings() function
*
* This function is used for debugging the configuration.
* It prints the actual configuration to the serial port.
*/
#ifdef DEBUG
void print_EEPROM_Settings() {
    Serial.print("IP: ");
    for(int i = 0; i<4; i++) {
      Serial.print(eeprom_config.ip[i]);
      if (i<3) {
        Serial.print('.');
      }
    }
    Serial.println();
  
    Serial.print("Subnet: ");
    for(int i = 0; i<4; i++) {
      Serial.print(eeprom_config.subnet[i]);
      if (i<3) {
        Serial.print('.');
      }
    }
    Serial.println();
    
    Serial.print("Gateway: ");
    for(int i = 0; i<4; i++) {
      Serial.print(eeprom_config.gateway[i]);
      if (i<3) {
        Serial.print('.');
      }
    }
    Serial.println();

    Serial.print("DNS Server: ");
    for(int i = 0; i<4; i++) {
      Serial.print(eeprom_config.dns_server[i]);
      if (i<3) {
        Serial.print('.');
      }
    }
    Serial.println();
    
    Serial.print("MAC: ");
    for (int a=0;a<6;a++) {
      Serial.print(eeprom_config.mac[a],HEX);
      if(a<5) {
        Serial.print(":");
      }
    }
    Serial.println();
    
    Serial.print("Webserver Port: ");
    Serial.println(eeprom_config.webserverPort);
    
    Serial.print("USE DHCP: ");
    Serial.println(eeprom_config.use_dhcp);
    
    Serial.print(" DHCP renew every ");
    Serial.print(eeprom_config.dhcp_refresh_minutes);
    Serial.println(" minutes");
    
    Serial.print("Config Set: ");
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

  if (interval == 0 ) 
  {
     interval = 1; 
  }
  
  if (eeprom_config.use_dhcp==1) 
  {
    if((millis() - last_dhcp_renew) >  interval_millis) {
      last_dhcp_renew=millis();
      dhcp_state = Ethernet.maintain();
    }
  }
}

#ifdef DEBUG
  #undef DEBUG
#endif

#endif
