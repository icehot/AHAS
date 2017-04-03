// 
// 
// 

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


/** OS **/

#ifdef DEBUG
#undef DEBUG
#endif

//#define DEBUG

Scheduler TaskScheduler;

/** Task Definitions **/
Task Task_Init(TASK_IMMEDIATE, TASK_ONCE, &Task_Init_Callback);
Task Task_Acquisition(1000, TASK_FOREVER, &Task_Acquisition_Callback);
Task Task_Display(1000, TASK_FOREVER, &Task_Display_Callback); //TBD
Task Task_Button(200, TASK_FOREVER, &Task_Button_Callback); //TBD
Task Task_Webduino(500, TASK_FOREVER, &Task_Webduino_Callback);
Task Task_Log(TASK_MINUTE, TASK_FOREVER, &Task_Log_Callback);
Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback);
Task Task_TimeSync(TASK_HOUR, TASK_FOREVER, &Task_TimeSync_Callback);
Task Task_ThingSpeak(TASK_MINUTE, TASK_FOREVER, &Task_ThingSpeak_Callback);

bool acqFirstRun = 1;

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
#ifdef USE_MENWIZZ
    init_MenWizz();
#else
    init_LCD();
#endif
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
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("#SOUND: Activated!"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#SOUND: Activated!"));
#endif
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
    if (acqFirstRun)
    {
		acqFirstRun = 0;
		Serial.println("enable");
        Task_Display.enable();
        Task_Webduino.enable();
        Task_Log.enable();
    }

    endRuntimeMeasurement(&RunTime.Task_Acquisition);
}

void Task_Button_Callback()
{

#ifdef USE_ANALOG_BTN
    int analogButtonState = readAnalogButton();

    /* If there is a button press */
    if (analogButtonState != MW_BTNULL)
    {
        /* react on this new button press by drawing it */
#ifdef USE_MENWIZZ
        tree.draw(analogButtonState);
#endif
    }
#endif
}

void Task_Display_Callback()
{
#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Display Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Display.getStartDelay());
#endif 

#ifdef USE_LCD
#ifdef USE_MENWIZZ
    tree.drawWoBtnCheck();
#else
    updateLCD();
#endif
#endif
}

void Task_Webduino_Callback()
{
#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Webduino Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Display.getStartDelay());
#endif 

    //startRuntimeMeasurement();

#ifdef USE_WEBDUINO
    WebduinoServerLoop();
#endif

    //endRuntimeMeasurement(&RunTime.Task_Webduino);

#ifdef DEBUG
    Serial.print(millis());
    Serial.print(F(" #WEB "));
    Serial.print(F("Delay: "));
    Serial.println(Task_Display.getStartDelay());
    printRuntTime(&RunTime.Task_Webduino);
#endif
}

void Task_Log_Callback()
{
#ifdef DEBUG
    Serial.print(millis());
    Serial.println(F(" #OS: Log Task"));
    Serial.print(F("Delayed: "));
    Serial.println(Task_Log.getStartDelay());
#endif

#ifdef USE_SD
    saveDataToLog();
#endif
}

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
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("#DHCP: Nothing Happened"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#DHCP: Nothing Happened"));
#endif
        break;
    case RenewFailed:
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("#DHCP: Renew Failed"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#DHCP: Renew Failed"));
#endif
        break;
    case RenewSuccess:
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("#DHCP: Renew Success"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#DHCP: Renew Success"));
#endif
        break;
    case RebindFailed:
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("#DHCP: Rebind Failed"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#DHCP: Rebind Failed"));
#endif
        break;
    case RebindSuccess:
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("#DHCP: Rebind Success"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#DHCP: Rebind Success"));
#endif
        break;
    default:
#ifdef USE_SERIAL_MONITOR
        Serial.println(F("#DHCP: Unknown Error"));
#endif
#ifdef USE_SYS_LOG
        add2SysLog(F("#DHCP: Unknown Error"));
#endif
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

#ifdef DEBUG
#undef DEBUG
#endif
