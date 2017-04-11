/** OS **/

/** System Header Files **/
#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "App_DS1302.h"
#include "App_DHT11.h"
#include "App_BMP085.h"
#include "App_MS5611.h"
#include "App_Net.h"
#include "App_Webduino.h"
#include "App_NTP.h"
#include "App_ThingSpeak.h"
#include "App_LCD.h"
#include "App_RunTime.h"

/** Foreign Header Files **/
#include <MENWIZ.h>
#include <TimeX.h>
#include <Ethernet.h>

/** Own Header Files **/
#include "App_OS.h"

Scheduler TaskScheduler;

/** Task Definitions **/
Task Task_Init(TASK_IMMEDIATE, TASK_ONCE, &Task_Init_Callback);
Task Task_Acquisition(1000, TASK_FOREVER, &Task_Acquisition_Callback);

#ifdef USE_LCD
Task Task_Display(1000, TASK_FOREVER, &Task_Display_Callback); //TBD
#endif

#ifdef USE_ANALOG_BTN
Task Task_Button(200, TASK_FOREVER, &Task_Button_Callback); //TBD
#endif

#ifdef USE_WEBDUINO
Task Task_Webduino(500, TASK_FOREVER, &Task_Webduino_Callback);
#endif

#ifdef USE_SYS_LOG
Task Task_Log(TASK_MINUTE, TASK_FOREVER, &Task_Log_Callback);
#endif

#ifdef USE_ETH_SHIELD
Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback);
#endif

#ifdef USE_NTP
Task Task_TimeSync(TASK_HOUR, TASK_FOREVER, &Task_TimeSync_Callback);
#endif

#ifdef USE_THINGSPEAK
Task Task_ThingSpeak(TASK_MINUTE, TASK_FOREVER, &Task_ThingSpeak_Callback);
#endif

/** Function Definition **/

void Task_Init_Callback()
{
    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    init_LedBuiltIn();

    init_UART();

    #ifdef DEBUG_OS
    Serial.print(F("#OS INI @"));
	Serial.print(millis());
    Serial.print(F(" Delay: "));
    Serial.print(Task_Init.getStartDelay());
    #endif 

    #ifdef USE_SD
    init_SD();
    #endif 

    #ifdef USE_DS1302
    init_DS1302();
    #endif

    #ifdef USE_SOUND_DETECT
    init_SoundDetect();
    #endif 

    #ifdef USE_RELAY
    init_Relay();
    #endif 

    #ifdef USE_PIR
    init_PIR();
    #endif

    #ifdef USE_RGB
    init_RGB();
    #endif

    #ifdef USE_DHT11
    init_DHT11();
    #endif

    #ifdef USE_BMP085
    init_BMP085();
    #endif

    #ifdef USE_MS5611
    init_MS5611();
    #endif

    #ifdef USE_ETH_SHIELD
    init_NetSetup();
    #endif

    #ifdef USE_WEBDUINO
    init_Webduino();
    #endif

    #ifdef USE_NTP
    init_NTP();
    #endif

    #ifdef USE_LCD
    init_MenWizz();
    #endif

    #ifdef USE_BACKLIGHT
    init_BackLight();
    DataPool.LCD_BackLight = DEFAULT_BACKLIGHT;
    #endif

    #ifdef USE_CONTRAST
    init_Contrast();
    DataPool.LCD_Contrast = DEFAULT_CONTRAST;
    #endif

    #ifdef USE_THINGSPEAK
    init_ThingSpeak();
    #endif

    /* Enable further tasks*/
    Task_Acquisition.enable();

    #ifdef USE_NTP
    Task_TimeSync.enableDelayed(TASK_SECOND/2);
    #endif

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_Init);
    #endif

    #ifdef DEBUG_OS
    printRuntTime(&RunTime.Task_Init);
    #endif
}

void Task_Acquisition_Callback()
{
	#ifdef DEBUG_OS
	Serial.print(F("#OS ACQ @"));
	Serial.print(millis());
	Serial.print(F(" Delay: "));
	Serial.print(Task_Acquisition.getStartDelay());
	#endif 


    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    toggleLedBuiltIn();

    #ifdef USE_DHT11
    read_DHT11();
    #endif

    #ifdef USE_BMP085
    read_BMP085();
    #endif

    #ifdef USE_MS5611
    read_MS5611();
    #endif

    #ifdef USE_NTP
    read_time();
    #endif

    #ifdef USE_DS1302
    read_DS1302();
    #endif

    #ifdef USE_PIR
    get_PIR_State();
    #endif

    #ifdef USE_SOUND_DETECT   
    if (get_SoundDetect_State() == 1)
    {
        MONITOR_LOG_LN(F("#SOUND: Activated!"));
        clear_SoundDetect_State();

        /* Clear the pending interrupts */
        EIFR = 0x10;//INTF4 for pin 2
        /* Reenable the interrupt */
        attachInterrupt(digitalPinToInterrupt(PIN_SOUND_DETECT), ISR_SoundDetect, FALLING);
    }
    #endif

    #ifdef USE_RGB
    analogWrite(PIN_RGBLED_R, DataPool.RGB_Red);
    analogWrite(PIN_RGBLED_G, DataPool.RGB_Green);
    analogWrite(PIN_RGBLED_B, DataPool.RGB_Blue);
    #endif

    #ifdef USE_BACKLIGHT
    setBackLight(DataPool.LCD_BackLight);
    #endif

    #ifdef USE_CONTRAST
    setContrast(DataPool.LCD_Contrast);
    #endif

    if(Task_Acquisition.isFirstIteration())
    {
        #ifdef USE_LCD
        Task_Display.enable();
        #endif

        #ifdef USE_ANALOG_BTN
        Task_Button.enable();
        #endif
        
        #ifdef USE_WEBDUINO
        Task_Webduino.enable();
        #endif
        
        #ifdef USE_SYS_LOG
        Task_Log.enable();
        #endif

        #ifdef USE_THINGSPEAK
        Task_ThingSpeak.enableDelayed(TASK_MINUTE);
        #endif
    }

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_Acquisition);
    #endif

	#ifdef DEBUG_OS
	printRuntTime(&RunTime.Task_Acquisition);
	#endif
}

#ifdef USE_ANALOG_BTN
void Task_Button_Callback()
{
	#ifdef DEBUG_OS
	Serial.print(F("#OS BUT @"));
	Serial.print(millis());
	Serial.print(F(" Delay: "));
	Serial.print(Task_Button.getStartDelay());
	#endif 

    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    int analogButtonState = readAnalogButton();

    /* If there is a button press */
    if (analogButtonState != MW_BTNULL)
    {
        /* react on this new button press by drawing it */
        #ifdef USE_LCD
        tree.draw(analogButtonState);
        #endif
    }

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_Button);
    #endif

	#ifdef DEBUG_OS
	printRuntTime(&RunTime.Task_Button);
	#endif
}
#endif

#ifdef USE_LCD
void Task_Display_Callback()
{
	#ifdef DEBUG_OS
	Serial.print(F("#OS LCD @"));
	Serial.print(millis());
	Serial.print(F(" Delay: "));
	Serial.print(Task_Display.getStartDelay());
	#endif 

    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    tree.drawWoBtnCheck();

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_Display);
    #endif

    #ifdef DEBUG_OS
	printRuntTime(&RunTime.Task_Display);
    #endif
}

#endif

#ifdef USE_WEBDUINO
void Task_Webduino_Callback()
{
    #ifdef DEBUG_OS
    Serial.print(F("#OS WEB @"));
    Serial.print(millis());
    Serial.print(F(" Delay: "));
    Serial.print(Task_Webduino.getStartDelay());
    #endif 

    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    WebduinoServerLoop();

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_Webduino);
    #endif

    #ifdef DEBUG_OS
    printRuntTime(&RunTime.Task_Webduino);
    #endif
}
#endif

#ifdef USE_SYS_LOG
void Task_Log_Callback()
{
    #ifdef DEBUG_OS
    Serial.print(F("#OS LOG @"));
    Serial.print(millis());
    Serial.print(F(" Delay: "));
    Serial.print(Task_Log.getStartDelay());
    #endif 

    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    saveDataToLog();

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_Log);
    #endif

    #ifdef DEBUG_OS
    printRuntTime(&RunTime.Task_Log);
    #endif

    #ifdef USE_RUNTIME
    Serial.println("\nRuntime Statistics: \n");
    Serial.println("INI:");
    printRuntTimeStatistics(&RunTime.Task_Init);
    Serial.println("ACQ:");
    printRuntTimeStatistics(&RunTime.Task_Acquisition);
    Serial.println("WEB:");
    printRuntTimeStatistics(&RunTime.Task_Webduino);
    Serial.println("BUT:");
    printRuntTimeStatistics(&RunTime.Task_Button);
    Serial.println("LCD:");
    printRuntTimeStatistics(&RunTime.Task_Display);
    Serial.println("NET:");
    printRuntTimeStatistics(&RunTime.Task_RenewDHCP);
    Serial.println("NTP:");
    printRuntTimeStatistics(&RunTime.Task_TimeSync);
    Serial.println("TSP:");
    printRuntTimeStatistics(&RunTime.Task_ThingSpeak);
    Serial.println("LOG:");
    printRuntTimeStatistics(&RunTime.Task_Log);
    Serial.println("----\n");
    #endif
}
#endif

#ifdef USE_ETH_SHIELD
void Task_RenewDHCP_Callback()
{
    #ifdef DEBUG_OS
    Serial.print(F("\n#OS NET @"));
    Serial.print(millis());
    Serial.print(F(" Delay: "));
    Serial.print(Task_RenewDHCP.getStartDelay());
    #endif 

    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    /* Renew DHCP lease */

    dhcp_state = Ethernet.maintain();

    switch (dhcp_state)
    {
    case NothingHappened:
        MONITOR_LOG_LN(F("#DHCP: Nothing Happened"));
        break;

    case RenewFailed:
        MONITOR_LOG_LN(F("#DHCP: Renew Failed"));
        break;

    case RenewSuccess:
        MONITOR_LOG_LN(F("#DHCP: Renew Success"));
        break;

    case RebindFailed:
        MONITOR_LOG_LN(F("#DHCP: Rebind Failed"));
        break;

    case RebindSuccess:
        MONITOR_LOG_LN(F("#DHCP: Rebind Success"));
        break;

    default:
        MONITOR_LOG_LN(F("#DHCP: Unknown Error"));
        break;
    }

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_RenewDHCP);
    #endif

    #ifdef DEBUG_OS
    printRuntTime(&RunTime.Task_RenewDHCP);
    #endif
}
#endif

#ifdef USE_NTP
void Task_TimeSync_Callback()
{
    #ifdef DEBUG_OS
    Serial.print(F("#OS NTP @"));
    Serial.print(millis());
    Serial.print(F(" Delay: "));
    Serial.print(Task_TimeSync.getStartDelay());
    #endif 

    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    switch (timeStatus())
    {
    case timeSet:
        #ifdef USE_DS1302
        autoTimeSync();
        #endif
        break;

    default:
        /* NTP is not available */
        break;
    }

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_TimeSync);
    #endif

    #ifdef DEBUG_OS
    printRuntTime(&RunTime.Task_TimeSync);
    #endif
}
#endif

#ifdef USE_THINGSPEAK
void Task_ThingSpeak_Callback()
{
    #ifdef DEBUG_OS
    Serial.print(F("\n#OS TSP @"));
    Serial.print(millis());
    Serial.print(F(" Delay: "));
    Serial.print(Task_ThingSpeak.getStartDelay());
    #endif 

    #ifdef USE_RUNTIME
    startRuntimeMeasurement();
    #endif

    updateThingSpeak();

    #ifdef USE_RUNTIME
    endRuntimeMeasurement(&RunTime.Task_ThingSpeak);
    #endif

    #ifdef DEBUG_OS
    printRuntTime(&RunTime.Task_ThingSpeak);
    #endif
}
#endif
