/** LCD - HD44780 Display **/

#ifdef USE_LCD
#include <LiquidCrystal.h>

#define NR_OF_SCREENS 2;

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

byte degree[8] = {
  0b00100,
  0b01010,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};

void printDigits(int digits);

void init_LCD()
{
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
  
  Serial.println("#INIT: LCD => DONE");
}

void updateLCD() 
{
  static byte screen = 0;

  lcd.clear();
  lcd.setCursor(0, 0);//top left corner

  switch(screen)
  {
    case 0:
      /* First Row */
      #ifdef USE_DS1302
      lcd.print("Date: ");
      printDigits(DataPool.DS1302_Year);lcd.print("/");
      printDigits(DataPool.DS1302_Month);lcd.print("/");
      printDigits(DataPool.DS1302_Day);
      
    
      /* Second Row */
      lcd.setCursor(0,1);//bottom left corner
      lcd.print("Time: ");
      printDigits(DataPool.DS1302_Hour);lcd.print(":");
      printDigits(DataPool.DS1302_Minute);
      #endif
    break;

    case 1:
      /* First Row */
      #ifdef USE_MS5611
      lcd.print("T: ");
      lcd.print(DataPool.MS5611_Temperature,1);
      lcd.write(byte(0));
      #endif
      
      #ifdef USE_DHT11
      lcd.print("C H: ");
      lcd.print(DataPool.DHT11_Humidity);
      lcd.print("%");
      #endif
     
      /* Second Row */
      #ifdef USE_MS5611
      lcd.setCursor(0, 1);//bottom left corner
      lcd.print("P: ");
      lcd.print(DataPool.MS5611_Pressure/(float)100);
      lcd.print(" mBar");
      #endif
      
    break;
      
    case 2:
      /* First Row */
      /* Second Row */
    break;

    default:
    break;
  }
  
  screen++;
  screen %= NR_OF_SCREENS;
}

void printDigits(int digits)
{
  // utility for digital clock display: prints leading 0
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}
#endif
