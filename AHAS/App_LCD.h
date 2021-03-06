/** LCD - HD44780 Display **/

#ifndef _LCD_h
#define _LCD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MENWIZ.h"

#ifdef USE_LCD
extern menwiz tree;

void printDigits(int digits);
void updateLCD();
void init_MenWizz();
void initLCD();
void showStatusOnLCD(char * status);

#endif
#endif

