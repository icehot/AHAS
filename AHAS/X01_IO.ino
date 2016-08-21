/** I/O **/
void add2SysLog(char * entry);

void softReset()
{
    asm volatile ("jmp 0");
}

void init_UART()
{
  //Serial monitor initialization
  Serial.begin(9600);
  
  #ifdef USE_SERIAL_MONITOR
    Serial.println("### AHAS - Arduino Home Automation System ###");
    Serial.println("#INIT: UART => DONE");
  #endif
}


#ifdef USE_DEBUG_LED
void init_DEBUG_LED()
{
  pinMode(PIN_DEBUG_LED, OUTPUT);
  digitalWrite(PIN_DEBUG_LED,LOW);

  #ifdef USE_SERIAL_MONITOR
    Serial.println("#INIT: DEBUG_LED => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: DEBUG_LED => DONE");
  #endif
}
#endif

#ifdef USE_FACTDEF_BTN
void init_FACT_DEF_BTN()
{
  pinMode(PIN_RESET, INPUT);

  #ifdef USE_SERIAL_MONITOR
    Serial.println("#INIT: FACT_DEF_BTN => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: FACT_DEF_BTN => DONE");
  #endif
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
    Serial.println("#INIT: RGB => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: RGB => DONE");
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
    Serial.println("#INIT: RElAY => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: RElAY => DONE");
  #endif
}
#endif 

/** PIR Sensor **/
#ifdef USE_PIR
void init_PIR()
{
  pinMode(PIN_PIR_SENSOR, INPUT);

  #ifdef USE_SERIAL_MONITOR
    Serial.println("#INIT: PIR => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: PIR => DONE");
  #endif
}

void get_PIR_State()
{
  DataPool.PIR_State = digitalRead(PIN_PIR_SENSOR);
  Serial.print("#PIR: ");
  Serial.println(DataPool.PIR_State);
}
#endif

/** PIR Sensor **/
#ifdef USE_SOUND_DETECT
void init_SoundDetect()
{
  pinMode(PIN_SOUND_DETECT, INPUT);

  #ifdef USE_SERIAL_MONITOR
    Serial.println("#INIT: SOUND DETECT => DONE");
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog("#INIT: SOUND DETECT => DONE");
  #endif
}

void get_SoundDetect_State()
{
  DataPool.SOUND_State = digitalRead(PIN_SOUND_DETECT);
  Serial.print("#SOUND: ");
  Serial.println(DataPool.SOUND_State);
}
#endif
