/** I/O **/

#include "AHAS_Config.h"
#include "App_IO.h"
#include "App_SD.h"
#include "App_Var.h"
#include "MENWIZ.h"

void add2SysLog(char * entry);
void add2SysLog(const __FlashStringHelper * entry);

void softReset()
{
    asm("jmp 0"); //asm volatile ("jmp 0");
}

void init_LedBuiltIn()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void toggleLedBuiltIn()
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void init_UART()
{
    //Serial monitor initialization
    Serial.begin(115200);

#ifdef USE_SERIAL_MONITOR
    Serial.println(F("### AHAS - Arduino Home Automation System ###"));
    Serial.println(F("#INIT: UART => DONE"));
#endif
}

#ifdef USE_BACKLIGHT
void init_BackLight()
{
    pinMode(PIN_BACKLIGHT, OUTPUT);
}

void setBackLight(byte value)
{
    analogWrite(PIN_BACKLIGHT, value);
}
#endif

#ifdef USE_CONTRAST
void init_Contrast()
{
    pinMode(PIN_CONTRAST, OUTPUT);
}

void setContrast(byte value)
{
    analogWrite(PIN_CONTRAST, value);
}
#endif

#ifdef USE_ANALOG_BTN
int readAnalogButton()
{
    byte retVal = MW_BTNULL;
    static byte prevVal = MW_BTNULL;
    int val = analogRead(PIN_ANALOG_BUTTON);

    if (val > LL_BTNULL && val < UL_BTNULL)
    {/* No button */
        retVal = MW_BTNULL;
    }
    else if (val >= LL_BTE && val <= UL_BTE)
    {/* Escape button */
        retVal = MW_BTE;
    }
    else if (val > LL_BTD && val < UL_BTD)
    {/* Down button */
        retVal = MW_BTD;
    }
    else if (val > LL_BTR && val < UL_BTR)
    {/* Right button */
        retVal = MW_BTR;
    }
    else if (val > LL_BTC && val < UL_BTC)
    {/* Confirm button */
        retVal = MW_BTC;
    }
    else if (val > LL_BTL && val < UL_BTL)
    {/* Left button */
        retVal = MW_BTL;
    }
    else if (val > LL_BTU && val < UL_BTU)
    {/* Up button */
        retVal = MW_BTU;
    }
    else
    {
        /* Value out of limits of defined buttons */
        /* Default return value is already set */
    }

    if (retVal != MW_BTNULL && prevVal != MW_BTNULL)
    {
        prevVal = retVal;
        retVal = MW_BTNULL;
    }
    else
    {
        prevVal = retVal;
    }

    return (int)retVal;
}

#endif

#ifdef USE_RGB
void init_RGB()
{
    pinMode(PIN_RGBLED_R, OUTPUT);
    pinMode(PIN_RGBLED_G, OUTPUT);
    pinMode(PIN_RGBLED_B, OUTPUT);

    analogWrite(PIN_RGBLED_R, 128); //zold
    analogWrite(PIN_RGBLED_G, 128); //piros
    analogWrite(PIN_RGBLED_B, 128);

#ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: RGB => DONE"));
#endif
#ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: RGB => DONE"));
#endif
}
#endif

/** relay module **/
#ifdef USE_RELAY
void init_Relay()
{
    pinMode(PIN_RELAY1, OUTPUT);
    pinMode(PIN_RELAY2, OUTPUT);
    pinMode(PIN_RELAY3, OUTPUT);
    pinMode(PIN_RELAY4, OUTPUT);

    digitalWrite(PIN_RELAY1, HIGH);
    digitalWrite(PIN_RELAY2, HIGH);
    digitalWrite(PIN_RELAY3, HIGH);
    digitalWrite(PIN_RELAY4, HIGH);

#ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: RElAY => DONE"));
#endif
#ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: RElAY => DONE"));
#endif
}
#endif 

/** PIR Sensor **/
#ifdef USE_PIR
void init_PIR()
{
    pinMode(PIN_PIR_SENSOR, INPUT);

#ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: PIR => DONE"));
#endif
#ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: PIR => DONE"));
#endif
}

void get_PIR_State()
{
    DataPool.PIR_State = digitalRead(PIN_PIR_SENSOR);
    //Serial.print("#PIR: ");
    //Serial.println(DataPool.PIR_State);
}
#endif

/** Sound Sensor **/
#ifdef USE_SOUND_DETECT

void ISR_SoundDetect()
{
    DataPool.SOUND_State = 1;
    /* Disable the interrupt for a while, debouncing*/
    detachInterrupt(digitalPinToInterrupt(PIN_SOUND_DETECT));
}
void init_SoundDetect()
{
    pinMode(PIN_SOUND_DETECT, INPUT);

    /* Clear the interrupt flag */
    EIFR |= 0x10;//INTF4 for pin 2
    /* Enable the interrupt */
    attachInterrupt(digitalPinToInterrupt(PIN_SOUND_DETECT), ISR_SoundDetect, FALLING);

#ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: SOUND DETECT => DONE"));
#endif
#ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: SOUND DETECT => DONE"));
#endif
}

byte get_SoundDetect_State()
{
    return DataPool.SOUND_State;
}

void clear_SoundDetect_State()
{
    DataPool.SOUND_State = 0;
}
#endif

void filterJacob(float * filtValue, float newValue, byte order)
{
    *filtValue = ((*filtValue)*(order - 1)) / order + newValue / order;
}
