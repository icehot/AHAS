// 
//    FILE: DS1302.cpp
// VERSION: 0.1
// PURPOSE: DS1302 Real-time clock library for Arduino
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

#include "DS1302.h"
/** DS1302 sensor **/

// Below there are the utility functions to get data from the DS1302 sensor.

//Initialize the DS1302 sensor
void DS1302::init(unsigned char pinSCLK, unsigned char pinIO, unsigned char pinCE)
{
  this->pinSCLK = pinSCLK;
  this->pinIO = pinIO;
  this->pinCE = pinCE;
}

//Set date and time for DS1302
void DS1302::setTimeAndDate(uint8_t year, uint8_t month, uint8_t dayofmonth, uint8_t dayofweek, uint8_t hours, uint8_t minutes, uint8_t seconds )
{
  RtcDataType rtc;

  // Start by clearing the Write Protect bit
  // Otherwise the clock data cannot be written
  // The whole register is written, 
  // but the WP-bit is the only bit in that register.
  write (DS1302_ENABLE, 0);

  // Disable Trickle Charger.
  write (DS1302_TRICKLE, 0x00);
 
  // Set a time and date
  // This also clears the CH (Clock Halt) bit, 
  // to start the clock.

  // Fill the structure with zeros to make 
  // any unused bits zero
  memset ((char *) &rtc, 0, sizeof(rtc));

  rtc.Seconds    = bin2bcd_l( seconds);
  rtc.Seconds10  = bin2bcd_h( seconds);
  rtc.CH         = 0;      // 1 for Clock Halt, 0 to run;
  rtc.Minutes    = bin2bcd_l( minutes);
  rtc.Minutes10  = bin2bcd_h( minutes);
  // To use the 12 hour format,
  // use it like these four lines:
  //    rtc.h12.Hour   = bin2bcd_l( hours);
  //    rtc.h12.Hour10 = bin2bcd_h( hours);
  //    rtc.h12.AM_PM  = 0;     // AM = 0
  //    rtc.h12.hour_12_24 = 1; // 1 for 24 hour format
  rtc.h24.Hour   = bin2bcd_l( hours);
  rtc.h24.Hour10 = bin2bcd_h( hours);
  rtc.h24.hour_12_24 = 0; // 0 for 24 hour format
  rtc.Date       = bin2bcd_l( dayofmonth);
  rtc.Date10     = bin2bcd_h( dayofmonth);
  rtc.Month      = bin2bcd_l( month);
  rtc.Month10    = bin2bcd_h( month);
  rtc.Day        = dayofweek;
  rtc.Year       = bin2bcd_l( year);
  rtc.Year10     = bin2bcd_h( year);
  rtc.WP = 0;  

  // Write all clock data at once (burst mode).
  clock_burst_write( (uint8_t *) &rtc);
}

//Get date and time for DS1302
void DS1302::getTimeAndDate(uint8_t* year, uint8_t* month, uint8_t* dayofmonth, uint8_t* dayofweek, uint8_t* hours, uint8_t* minutes, uint8_t* seconds)
{
	RtcDataType rtc;

	// Read all clock data at once (burst mode).
	clock_burst_read( (uint8_t *) &rtc);
  
	*year       = bcd2bin( rtc.Year10, rtc.Year);
	*month      = bcd2bin( rtc.Month10, rtc.Month);
	*dayofmonth = bcd2bin( rtc.Date10, rtc.Date);
	
	*dayofweek  = rtc.Day;
  
	*hours   = bcd2bin( rtc.h24.Hour10, rtc.h24.Hour);
	*minutes = bcd2bin( rtc.Minutes10, rtc.Minutes);
	*seconds = bcd2bin( rtc.Seconds10, rtc.Seconds);
}

// --------------------------------------------------------
// DS1302_clock_burst_read
//
// This function reads 8 bytes clock data in burst mode
// from the DS1302.
//
// This function may be called as the first function, 
// also the pinMode is set.
//
void DS1302::clock_burst_read( uint8_t *p)
{
  int i;

  start();

  // Instead of the address, 
  // the CLOCK_BURST_READ command is issued
  // the I/O-line is released for the data
  togglewrite( DS1302_CLOCK_BURST_READ, true);  

  for( i=0; i<8; i++)
  {
    *p++ = toggleread();
  }
  stop();
}


// --------------------------------------------------------
// DS1302_clock_burst_write
//
// This function writes 8 bytes clock data in burst mode
// to the DS1302.
//
// This function may be called as the first function, 
// also the pinMode is set.
//
void DS1302::clock_burst_write( uint8_t *p)
{
  int i;

  start();

  // Instead of the address, 
  // the CLOCK_BURST_WRITE command is issued.
  // the I/O-line is not released
  togglewrite( DS1302_CLOCK_BURST_WRITE, false);  

  for( i=0; i<8; i++)
  {
    // the I/O-line is not released
    togglewrite( *p++, false);  
  }
  
  stop();
}


// --------------------------------------------------------
// DS1302_read
//
// This function reads a byte from the DS1302 
// (clock or ram).
//
// The address could be like "0x80" or "0x81", 
// the lowest bit is set anyway.
//
// This function may be called as the first function, 
// also the pinMode is set.
//
uint8_t DS1302::read(int address)
{
  uint8_t data;

  // set lowest bit (read bit) in address
  bitSet( address, DS1302_READBIT);  

  start();
  // the I/O-line is released for the data
  togglewrite( address, true);  
  data = toggleread();
  stop();

  return (data);
}


// --------------------------------------------------------
// DS1302_write
//
// This function writes a byte to the DS1302 (clock or ram).
//
// The address could be like "0x80" or "0x81", 
// the lowest bit is cleared anyway.
//
// This function may be called as the first function, 
// also the pinMode is set.
//
void DS1302::write( int address, uint8_t data)
{
  // clear lowest bit (read bit) in address
  bitClear( address, DS1302_READBIT);   

  start();
  // don't release the I/O-line
  togglewrite( address, false); 
  // don't release the I/O-line
  togglewrite( data, false); 
  stop();  
}


// --------------------------------------------------------
// _DS1302_start
//
// A helper function to setup the start condition.
//
// An 'init' function is not used.
// But now the pinMode is set every time.
// That's not a big deal, and it's valid.
// That's not a big deal, and it's valid.
// At startup, the pins of the Arduino are high impedance.
// Since the DS1302 has pull-down resistors, 
// the signals are low (inactive) until the DS1302 is used.
void DS1302::start( void)
{
  digitalWrite( pinCE, LOW); // default, not enabled
  pinMode( pinCE, OUTPUT);  

  digitalWrite( pinSCLK, LOW); // default, clock low
  pinMode( pinSCLK, OUTPUT);

  pinMode( pinIO, OUTPUT);

  digitalWrite( pinCE, HIGH); // start the session
  delayMicroseconds( 4);           // tCC = 4us
}


// --------------------------------------------------------
// _DS1302_stop
//
// A helper function to finish the communication.
//
void DS1302::stop(void)
{
  // Set CE low
  digitalWrite( pinCE, LOW);

  delayMicroseconds( 4);           // tCWH = 4us
}


// --------------------------------------------------------
// _DS1302_toggleread
//
// A helper function for reading a byte with bit toggle
//
// This function assumes that the SCLK is still high.
//
uint8_t DS1302::toggleread( void)
{
  uint8_t i, data;

  data = 0;
  for( i = 0; i <= 7; i++)
  {
    // Issue a clock pulse for the next databit.
    // If the 'togglewrite' function was used before 
    // this function, the SCLK is already high.
    digitalWrite( pinSCLK, HIGH);
    delayMicroseconds( 1);

    // Clock down, data is ready after some time.
    digitalWrite( pinSCLK, LOW);
    delayMicroseconds( 1);        // tCL=1000ns, tCDD=800ns

    // read bit, and set it in place in 'data' variable
    bitWrite( data, i, digitalRead( pinIO)); 
  }
  return( data);
}


// --------------------------------------------------------
// _DS1302_togglewrite
//
// A helper function for writing a byte with bit toggle
//
// The 'release' parameter is for a read after this write.
// It will release the I/O-line and will keep the SCLK high.
//
void DS1302::togglewrite( uint8_t data, uint8_t release)
{
  int i;

  for( i = 0; i <= 7; i++)
  { 
    // set a bit of the data on the I/O-line
    digitalWrite( pinIO, bitRead(data, i));  
    delayMicroseconds( 1);     // tDC = 200ns

    // clock up, data is read by DS1302
    digitalWrite( pinSCLK, HIGH);     
    delayMicroseconds( 1);     // tCH = 1000ns, tCDH = 800ns

    if( release && i == 7)
    {
      // If this write is followed by a read, 
      // the I/O-line should be released after 
      // the last bit, before the clock line is made low.
      // This is according the datasheet.
      // I have seen other programs that don't release 
      // the I/O-line at this moment,
      // and that could cause a shortcut spike 
      // on the I/O-line.
      pinMode( pinIO, INPUT);

      // For Arduino 1.0.3, removing the pull-up is no longer needed.
      // Setting the pin as 'INPUT' will already remove the pull-up.
      // digitalWrite (DS1302_IO, LOW); // remove any pull-up  
    }
    else
    {
      digitalWrite( pinSCLK, LOW);
      delayMicroseconds( 1);       // tCL=1000ns, tCDD=800ns
    }
  }
}

//
// END OF FILE
//


