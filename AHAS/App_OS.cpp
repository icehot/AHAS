/** OS **/

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

#include "MENWIZ.h"
#include "TimeX.h"
#include <Ethernet.h>

#include "App_OS.h"


#ifdef DEBUG
#undef DEBUG
#endif

//#define DEBUG

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

Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback);

Task Task_TimeSync(TASK_HOUR, TASK_FOREVER, &Task_TimeSync_Callback);

#ifdef USE_THINGSPEAK
Task Task_ThingSpeak(TASK_MINUTE, TASK_FOREVER, &Task_ThingSpeak_Callback);
#endif

void Task_Init_Callback()
{
    init_LedBuiltIn();

    init_UART();

#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Init Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Init.getStartDelay());
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
	DataPool.LCD_BackLight = 128;
#endif

#ifdef USE_CONTRAST
    init_Contrast();
	DataPool.LCD_Contrast = 40;
#endif

    #ifdef USE_THINGSPEAK
    init_ThingSpeak();
    #endif

}

void Task_Acquisition_Callback()
{
	toggleLedBuiltIn();

#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Acquisition Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Acquisition.getStartDelay());
#endif 

    startRuntimeMeasurement();

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
    //sync_DS1302withNTP(); //Uncomment for DS1302 NTP sync
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

    /* Used for Analog Button Calibration
    Serial.println("");
    Serial.print("BTN:");
    Serial.println(analogRead(PIN_ANALOG_BUTTON));
    Serial.println("");
    */

	if(Task_Acquisition.isFirstIteration())
    {
        #ifdef USE_LCD
        Task_Display.enable();
        #endif
        
        #ifdef USE_WEBDUINO
        Task_Webduino.enable();
        #endif
        
        #ifdef USE_SYS_LOG
        Task_Log.enable();
        #endif
    }

    endRuntimeMeasurement(&RunTime.Task_Acquisition);
}

#ifdef USE_ANALOG_BTN
void Task_Button_Callback()
{
    int analogButtonState = readAnalogButton();

    /* If there is a button press */
    if (analogButtonState != MW_BTNULL)
    {
        /* react on this new button press by drawing it */
        #ifdef USE_LCD
        tree.draw(analogButtonState);
        #endif
    }
}
#endif

#ifdef USE_LCD
void Task_Display_Callback()
{
    #ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Display Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Display.getStartDelay());
    #endif 

    tree.drawWoBtnCheck();
}
#endif

#ifdef USE_WEBDUINO
void Task_Webduino_Callback()
{
    #ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Webduino Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Display.getStartDelay());
    #endif 

    //startRuntimeMeasurement();

    WebduinoServerLoop();

    //endRuntimeMeasurement(&RunTime.Task_Webduino);

    #ifdef DEBUG
    Serial.print(millis());
    Serial.print(F(" #WEB "));
    Serial.print(F("Delay: "));
    Serial.println(Task_Display.getStartDelay());
    printRuntTime(&RunTime.Task_Webduino);
    #endif
}
#endif

#ifdef USE_SYS_LOG
void Task_Log_Callback()
{
    #ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Log Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Log.getStartDelay());
    #endif

    saveDataToLog();
}
#endif

void Task_RenewDHCP_Callback()
{
#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Renew DHCP Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Log.getStartDelay());
#endif

#ifdef USE_ETH_SHIELD

    /* renew DHCP lease */

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
#endif
}

void Task_TimeSync_Callback()
{
#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Time Sync Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_TimeSync.getStartDelay());
#endif

#ifdef USE_NTP 
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
#endif
}

#ifdef USE_THINGSPEAK
void Task_ThingSpeak_Callback()
{
#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: ThingSpeak Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_ThingSpeak.getStartDelay());
#endif

#ifdef USE_THINGSPEAK
    updateThingSpeak();
#endif
}
#endif


#ifdef DEBUG
#undef DEBUG
#endif
