/** LCD - HD44780 Display **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_LCD.h"
#include "App_IO.h"
#include "App_SD.h"
#include "App_EEPROM.h"


#ifdef USE_LCD
#include <LiquidCrystal.h>

#include <MENWIZ.h>
#include <EEPROM.h>
#include <Ethernet.h>

/* Global variable definitions*/

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

menwiz tree;

#define NR_OF_SCREENS 10;

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

byte backslash[8] = {
    0b00000,
    0b10000,
    0b01000,
    0b00100,
    0b00010,
    0b00001,
    0b00000,
    0b00000
};

byte actScreen = 0;

void printDigits(int digits)
{
    // utility for digital clock display: prints leading 0
    if (digits < 10)
        lcd.print(F("0"));
    lcd.print(digits);
}

void updateLCD()
{
    switch (actScreen)
    {
    case 0:/* Date and Time with colon*/

        lcd.clear();
        lcd.setCursor(0, 0);//top left corner

        /* First Row */
#ifdef USE_DS1302
        lcd.print(F("Date: "));
        printDigits(DataPool.DS1302_Year + 2000); lcd.print(F("/"));
        printDigits(DataPool.DS1302_Month); lcd.print(F("/"));
        printDigits(DataPool.DS1302_Day);

        /* Second Row */
        lcd.setCursor(0, 1);//bottom left corner
        lcd.print(F("Time: "));
        printDigits(DataPool.DS1302_Hour); lcd.print(F(":"));
        printDigits(DataPool.DS1302_Minute);
#endif
        break;

    case 1: /* Clear colon*/
#ifdef USE_DS1302
        lcd.setCursor(8, 1);//colon 
        lcd.print(F(" "));
#endif
        break;

    case 2: /* Set colon*/
#ifdef USE_DS1302
        lcd.setCursor(8, 1);//colon 
        lcd.print(F(":"));
#endif
        break;

    case 3: /* Clear colon*/
#ifdef USE_DS1302
        lcd.setCursor(8, 1);//colon 
        lcd.print(F(" "));
#endif
        break;

    case 4: /* Display Values*/

        lcd.clear();
        lcd.setCursor(0, 0);//top left corner

        /* First Row */
#ifdef USE_MS5611
        lcd.print(F("T: "));
        lcd.print(DataPool.MS5611_Temperature, 1);
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
        lcd.print(DataPool.MS5611_Pressure / (float)100);
        lcd.print(F(" mBar"));

        lcd.setCursor(15, 1);//bottom right corner
        lcd.print(F("|"));
#endif

        break;

    case 5: /* slash */
        lcd.setCursor(15, 1);//bottom right corner
        lcd.print(F("/"));
        break;

    case 6: /* minus */
        lcd.setCursor(15, 1);//bottom right corner
        lcd.print(F("-"));
        break;

    case 7:/* backslash */
        lcd.setCursor(15, 1);//bottom right corner
        lcd.write(byte(1));
        break;

    case 8:/* Network*/
        lcd.clear();
        lcd.setCursor(0, 0);//top left corner

        #ifdef USE_ETH_SHIELD 
        /* First Row */
        lcd.print(F("IP: "));
        lcd.print(Ethernet.localIP());

        /* Second Row */
        lcd.setCursor(0, 1);//bottom left corner
        lcd.print(F("Port: "));
        lcd.print(eeprom_config.webserverPort); 
        #endif
        break;

    case 9: /*Network*/
        /* Delay for visibility */
        break;
    default:
        break;
    }

    actScreen++;
    actScreen %= NR_OF_SCREENS;
}

void init_MenWizz()
{
    _menu *r, *s1, *s2, *s3;

    tree.begin(&lcd, 16, 2); //declare lcd object and screen size to menwiz lib

    r = tree.addMenu(MW_ROOT, NULL, F("Settings"));
    s1 = tree.addMenu(MW_VAR, r, F("Contrast"));
    s1->addVar(MW_AUTO_BYTE, &DataPool.LCD_Contrast, 0, 255, 5);
    s2 = tree.addMenu(MW_VAR, r, F("Backlight"));
    s2->addVar(MW_AUTO_BYTE, &DataPool.LCD_BackLight, 0, 255, 10);

    #ifdef USE_ANALOG_BTN
    tree.addUsrNav(readAnalogButton, 6);/* Since the Memwizz draw was modified to event driven operation, probably it is not necessary */
    #endif

    tree.addUsrScreen(updateLCD, 5000);

    strcpy(tree.sbuf, "  ARDUINO HOME\n AUTOMATION SYS\n");
    tree.addSplash((char *)tree.sbuf, 5000);

    lcd.createChar(0, degree);
    lcd.createChar(1, backslash);

    MONITOR_LOG_LN(F("#INIT: LCD => DONE"));
}
#endif

