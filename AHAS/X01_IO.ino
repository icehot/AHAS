/** I/O **/

void softReset()
{
    asm volatile ("jmp 0");
}

void init_UART()
{
  //Serial monitor initialization
  Serial.begin(9600);

  Serial.println("#INIT: UART => DONE");
}


#ifdef USE_DEBUG_LED
void init_DEBUG_LED()
{
  pinMode(PIN_DEBUG_LED, OUTPUT);
  digitalWrite(PIN_DEBUG_LED,LOW);
  Serial.println("#INIT: DEBUG_LED => DONE");
}
#endif

#ifdef USE_FACTDEF_BTN
void init_FACT_DEF_BTN()
{
  pinMode(PIN_RESET, INPUT);
  Serial.println("#INIT: FACT_DEF_BTN => DONE");
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

  Serial.println("#INIT: IO => DONE");
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

  Serial.println("#INIT: RElAY => DONE");
}
#endif 
