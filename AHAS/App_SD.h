// SDUtil.h

#ifndef _SDUTIL_h
#define _SDUTIL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wstring.h>

#ifdef USE_SD
	void init_SD();
	void saveDataToLog();
	void add2SysLogWOTimeStamp(const char * entry);
	void add2SysLogWOTimeStamp(const __FlashStringHelper * entry);
	void add2SysLog(const __FlashStringHelper * entry);
	void add2SysLog(const char * entry);
#endif

#endif

