/** LCD - HD44780 Display **/

#ifdef USE_LCD
#include <LiquidCrystal.h>

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

#ifdef USE_MENWIZZ
#include <MENWIZ.h>
#include <EEPROM.h>

menwiz tree;
int  list,sp=110;

void DisplayValues()
{
  static  char buf[7];
  
  /* First line */
  strcpy(tree.sbuf,"Date: "); 
  strcat(tree.sbuf,itoa((int)DataPool.DS1302_Year,buf,10)); 
  strcat(tree.sbuf,"/");
  strcat(tree.sbuf,itoa((int)DataPool.DS1302_Month,buf,10)); 
  strcat(tree.sbuf,"/");
  strcat(tree.sbuf,itoa((int)DataPool.DS1302_Day,buf,10)); 
  strcat(tree.sbuf,"\n");

  /* Second line */
  strcat(tree.sbuf,"Time: "); 
  strcat(tree.sbuf,itoa((int)DataPool.DS1302_Hour,buf,10)); 
  strcat(tree.sbuf,":");
  strcat(tree.sbuf,itoa((int)DataPool.DS1302_Minute,buf,10)); 
  strcat(tree.sbuf,"\n");
  
  tree.drawUsrScreen(tree.sbuf);
}


void init_MenWizz()
{
  _menu *r,*s1,*s2;
    
  tree.begin(&lcd,16,2); //declare lcd object and screen size to menwiz lib

  r=tree.addMenu(MW_ROOT,NULL,F("Settings"));
    s1=tree.addMenu(MW_VAR,r, F("Contrast"));
      s1->addVar(MW_AUTO_BYTE,&DataPool.LCD_Contrast,0,255,10);  
    s2=tree.addMenu(MW_VAR,r, F("Backlight"));
      s2->addVar(MW_AUTO_BYTE,&DataPool.LCD_BackLight,0,255,10);    
    
    tree.addUsrNav(readAnalogButton, 6);
    tree.addUsrScreen(DisplayValues,5000);

    strcpy(tree.sbuf,"MENWIZ TEST \n V 3.13" );
    tree.addSplash((char *) tree.sbuf, 5000);
}



#else

#define NR_OF_SCREENS 2;

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
  
  #ifdef USE_SERIAL_MONITOR
    Serial.println(F("#INIT: LCD => DONE"));
  #endif
  #ifdef USE_SYS_LOG
    add2SysLog(F("#INIT: LCD => DONE"));
  #endif
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
      lcd.print(F("Date: "));
      printDigits(DataPool.DS1302_Year);lcd.print(F("/"));
      printDigits(DataPool.DS1302_Month);lcd.print(F("/"));
      printDigits(DataPool.DS1302_Day);
      
      /* Second Row */
      lcd.setCursor(0,1);//bottom left corner
      lcd.print(F("Time: "));
      printDigits(DataPool.DS1302_Hour);lcd.print(F(":"));
      printDigits(DataPool.DS1302_Minute);
      #endif
    break;

    case 1:
      /* First Row */
      #ifdef USE_MS5611
      lcd.print(F("T: "));
      lcd.print(DataPool.MS5611_Temperature,1);
      lcd.write(byte(0));
      #endif
      
      #ifdef USE_DHT11
      lcd.print(F("C H: "));
      lcd.print((byte)DataPool.DHT11_Humidity);
      lcd.print(F("%"));
      #endif
     
      /* Second Row */
      #ifdef USE_MS5611
      lcd.setCursor(0, 1);//bottom left corner
      lcd.print(F("P: "));
      lcd.print(DataPool.MS5611_Pressure/(float)100);
      lcd.print(F(" mBar"));
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
    lcd.print(F("0"));
  lcd.print(digits);
}
#endif
#endif
