void init_DS1302()
{
    ds1302.init(DS1302_SCLK_PIN, DS1302_IO_PIN, DS1302_CE_PIN);
    ds1302.setTimeAndDate(99,12,31,1,23,58,58);
}

void read_DS1302()
{
    ds1302.getTimeAndDate(&Rtc.year, &Rtc.month, &Rtc.dayofmonth, &Rtc.dayofweek, &Rtc.hours, &Rtc.minutes, &Rtc.seconds);
    Serial.println();
    Serial.print(2000+Rtc.year);Serial.print(".");Serial.print(Rtc.month);Serial.print(".");Serial.print(Rtc.dayofmonth); Serial.print(" ");
    Serial.print(Rtc.hours);Serial.print(":");Serial.print(Rtc.minutes);Serial.print(":");Serial.print(Rtc.seconds);Serial.println();
}
