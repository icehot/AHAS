#include <LiquidCrystal.h>

#define NR_OF_SCREENS 2;

LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

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

void init_LCD()
{
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
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
      lcd.print("Date: ");
      printDigits(DataPool.DS1302_Year);lcd.print("/");
      printDigits(DataPool.DS1302_Month);lcd.print("/");
      printDigits(DataPool.DS1302_Day);
    
      /* Second Row */
      lcd.setCursor(0,1);//bottom left corner
      lcd.print("Time: ");
      printDigits(DataPool.DS1302_Hour);lcd.print(":");
      printDigits(DataPool.DS1302_Minute);
    break;

    case 1:
      /* First Row */
      lcd.print("T: ");
      lcd.print(DataPool.MS5611_Temperature,1);
      lcd.write(byte(0));
      lcd.print("C H: ");
      lcd.print(DataPool.DHT11_Humidity);
      lcd.print("%");
     
      /* Second Row */
      lcd.setCursor(0, 1);//bottom left corner
      lcd.print("P: ");
      lcd.print(DataPool.MS5611_Pressure/(float)100);
      lcd.print(" mBar");
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
