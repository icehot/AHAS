// 
//    FILE: BMP085.h
// VERSION: 0.1
// PURPOSE: Barometric Pressure and Temperature Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: 
//
//     URL: 
//
// HISTORY:
// Orban Balazs - Original version
// 
// 

#ifndef BMP085_H
#define BMP085_H

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include "Wire.h"

#define BMP085LIB_VERSION "0.1"


class BMP085
{
	public:
		/* Functions*/
		void init(char i2c_address);
		long readPressure();
		int  readTemperature();
		
		/* Variables */
		// oversampling setting
		// 0 = ultra low power
		// 1 = standard
		// 2 = high
		// 3 = ultra high resolution
		const unsigned char oversampling_setting = 3; //oversampling for measurement
		const unsigned char pressure_conversiontime[4] = { 5, 8, 14, 26 };  // delays for oversampling settings 0, 1, 2 and 3
		
			
	private:
		/* Functions*/
		void read();
		void getCalibrationData();
		void dumpCalibrationData();
		int  readIntRegister(unsigned char r);
		void writeRegister(unsigned char r, unsigned char v);
		long readUP();
		int  readUT();
		
		/* Variables */
		long pressure;
		int temperature;
		char i2c_address;
		
		// sensor registers from the BOSCH BMP085 datasheet
		int ac1;
		int ac2; 
		int ac3; 
		unsigned int ac4;
		unsigned int ac5;
		unsigned int ac6;
		int b1; 
		int b2;
		int mb;
		int mc;
		int md;
};
#endif
//
// END OF FILE
//




  
