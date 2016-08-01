/** Init I/O **/

void init_IO()
{
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RESET, INPUT);
 
  pinMode(PIN_RGBLED_R, OUTPUT);
  pinMode(PIN_RGBLED_G, OUTPUT);
  pinMode(PIN_RGBLED_B, OUTPUT);

  analogWrite(PIN_RGBLED_R, 128);
  analogWrite(PIN_RGBLED_G, 128);
  analogWrite(PIN_RGBLED_B, 128);
  digitalWrite(PIN_LED,LOW);

  Serial.println("#INIT: IO => DONE");
}

void init_UART()
{
  //Serial monitor initialization
  Serial.begin(9600);

  Serial.println("#INIT: UART => DONE");
}

/** relay module **/

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

void softReset()
{
    asm volatile ("jmp 0");
}

