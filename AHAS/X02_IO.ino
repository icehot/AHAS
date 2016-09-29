/** I/O **/
void add2SysLog(char * entry);
void add2SysLog(const __FlashStringHelper * entry);

void softReset()
{
    asm volatile ("jmp 0");
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


#ifdef USE_DEBUG_LED
void init_DEBUG_LED()
{
  pinMode(PIN_DEBUG_LED, OUTPUT);
  digitalWrite(PIN_DEBUG_LED,LOW);

  #ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: DEBUG_LED => DONE"));
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: DEBUG_LED => DONE"));
  #endif
}
#endif

#ifdef USE_FACTDEF_BTN
void init_FACT_DEF_BTN()
{
  pinMode(PIN_RESET, INPUT);

  #ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: FACT_DEF_BTN => DONE"));
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: FACT_DEF_BTN => DONE"));
  #endif
}
#endif

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

#ifdef USE_RGB
void init_RGB()
{
  pinMode(PIN_RGBLED_R, OUTPUT);
  pinMode(PIN_RGBLED_G, OUTPUT);
  pinMode(PIN_RGBLED_B, OUTPUT);

  analogWrite(PIN_RGBLED_R, 128);
  analogWrite(PIN_RGBLED_G, 128);
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

  digitalWrite(PIN_RELAY1,HIGH);
  digitalWrite(PIN_RELAY2,HIGH);
  digitalWrite(PIN_RELAY3,HIGH);
  digitalWrite(PIN_RELAY4,HIGH);

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
  attachInterrupt(digitalPinToInterrupt(PIN_SOUND_DETECT),ISR_SoundDetect, FALLING);

  #ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: SOUND DETECT => DONE"));
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: SOUND DETECT => DONE"));
  #endif
}

byte inline get_SoundDetect_State()
{
  return DataPool.SOUND_State;
}

void inline clear_SoundDetect_State()
{
  DataPool.SOUND_State = 0;
}
#endif

void inline filterJacob(float * filtValue,float newValue, byte order)
{
  *filtValue = ((*filtValue)*(order-1))/order + newValue/order;
}

