#include "rtc_mcp79410.h"
#include "rtc_time_sync.h"
#include "common_util.h"

char dataStr1[7],
     dataStr2[7],
     dataStr3[7],
     dataStr4[7],
     dataStr5[7],
     dataStr6[7],
     dataStr7[7];
                 
int writeTimeRtcNtpSync()
{
    if(verbose == 1 || verbose == 0)
    {
         Serial.printf("%s> NTP: %02d, %02d- %02d- %-2d %02d: %02d: %02d \n", __FUNCTION__,
                                                                              ntp_timezone_s.time_wday,
                                                                              ntp_timezone_s.time_day, 
                                                                              ntp_timezone_s.time_month,
                                                                              ntp_timezone_s.time_year - 2000,
                                                                              ntp_timezone_s.time_hr,                                                                           
                                                                              ntp_timezone_s.time_min,
                                                                              ntp_timezone_s.time_sec);
    }
    if(RTC_NTP_TIME_UPDATE_REQUEST == false)
    {
        return RTC_NTP_TIME_UPDATE_REQUEST_FAILED;
    }
    if(RTC_NTP_TIME_UPDATE_REQUEST == true)
    {
         uint8_t rtc_start_bit   = 0x80,
                 rtc_stop_bit    = 0x0,
                 rtc_osc_en_bit  = 0x20,
                 rtc_vbat_en_bit = 0x08;
              
         uint8_t rtc_start_second_set_address = 0,          
                 rtc_min_set_address   = 1,
                 rtc_hr_set_address    = 2,
                 rtc_wday_set_address  = 3,
                 rtc_day_set_address   = 4,
                 rtc_month_set_address = 5,
                 rtc_year_set_address  = 6;

        if(verbose == 1 || verbose == 0)
        {
              Serial.printf("%s> RTC[hex]: %x %x-%x-%x %x:%x:%x \n", __FUNCTION__,
                                                                     rtc_timezone_s.time_wday,
                                                                     rtc_timezone_s.time_day,
                                                                     rtc_timezone_s.time_month,
                                                                     rtc_timezone_s.time_year,
                                                                     rtc_timezone_s.time_hr,
                                                                     rtc_timezone_s.time_min,
                                                                     rtc_timezone_s.time_sec);
      
              Serial.printf("%s> NTP[hex]: %x %x-%x-%x %x:%x:%x \n", __FUNCTION__,
                                                                     ntp_timezone_s.time_wday,
                                                                     ntp_timezone_s.time_day,
                                                                     ntp_timezone_s.time_month,
                                                                     ntp_timezone_s.time_year - 2000,
                                                                     ntp_timezone_s.time_hr,
                                                                     ntp_timezone_s.time_min,
                                                                     ntp_timezone_s.time_sec);
                                                                
              Serial.printf("%s> RTC[dec]: %02d %02d-%02d-%02d %02d:%02d:%02d \n", __FUNCTION__,
                                                                                   rtc_timezone_s.time_wday,
                                                                                   rtc_timezone_s.time_day,
                                                                                   rtc_timezone_s.time_month,
                                                                                   rtc_timezone_s.time_year,
                                                                                   rtc_timezone_s.time_hr,
                                                                                   rtc_timezone_s.time_min,
                                                                                   rtc_timezone_s.time_sec);
      
              Serial.printf("%s> NTP[dec]: %02d %02d-%02d-%02d %02d:%02d:%02d \n", __FUNCTION__,
                                                                                   ntp_timezone_s.time_wday,
                                                                                   ntp_timezone_s.time_day,
                                                                                   ntp_timezone_s.time_month,
                                                                                   ntp_timezone_s.time_year - 2000,
                                                                                   ntp_timezone_s.time_hr,
                                                                                   ntp_timezone_s.time_min,
                                                                                   ntp_timezone_s.time_sec);

        }
        memset(dataStr1, NULL, 10);
        memset(dataStr2, NULL, 10);
        memset(dataStr3, NULL, 10);
        memset(dataStr4, NULL, 10);
        memset(dataStr5, NULL, 10);
        memset(dataStr6, NULL, 10);
        memset(dataStr7, NULL, 10);

        uint8_t value1, value2, value3, value4, value5, value6, value7;

        sprintf(dataStr1, "0x%d", ntp_timezone_s.time_sec);
        {
            char *p = dataStr1;
            sscanf(p, "%x", &value1);
            value1 |= rtc_start_bit;
        }
           
        sprintf(dataStr2, "0x%d", ntp_timezone_s.time_min);
        {
            char *p = dataStr2;
            sscanf(p, "%x", &value2);
        }
            
        sprintf(dataStr3, "0x%d", ntp_timezone_s.time_hr);
        {
           char *p = dataStr3;
           sscanf(p, "%x", &value3);
        } 
        sprintf(dataStr4, "0x%d", ntp_timezone_s.time_day);
        {
           char *p = dataStr4;
           sscanf(p, "%x", &value4);
        }
        sprintf(dataStr5, "0x%d", ntp_timezone_s.time_month);
        {
           char *p = dataStr5;
           sscanf(p, "%x", &value5);
        }  
        sprintf(dataStr6, "0x%d", (ntp_timezone_s.time_year - 2000));
        {
           char *p = dataStr6;
           sscanf(p, "%x", &value6);
        } 
        sprintf(dataStr7, "0x%d", ntp_timezone_s.time_wday);
        {
           char *p = dataStr7;
           sscanf(p, "%x", &value7);
           value7 |= rtc_osc_en_bit;
           value7 |= rtc_vbat_en_bit;      
        }

        if(verbose == 1 || verbose == 0)
        {
            Serial.printf("%s> dataStr: %s, %s-%s-%s %s:%s:%s \n", __FUNCTION__,
                                                                   dataStr7,
                                                                   dataStr4,
                                                                   dataStr5,
                                                                   dataStr6,
                                                                   dataStr3,
                                                                   dataStr2,
                                                                   dataStr1);
            
            Serial.printf("%s> Values: %d, %d-%d-%d %d:%d:%d \n", __FUNCTION__,
                                                                  value7,
                                                                  value4,
                                                                  value5,
                                                                  value6,
                                                                  value3,
                                                                  value2,
                                                                  value1); 
        }                                                      

        WriteRTCByte(0,rtc_stop_bit);       //STOP RTC
        WriteRTCByte(1, value2);            //MINUTE=18
        WriteRTCByte(2, value3);            //HOUR=8
        WriteRTCByte(3, value7);            //DAY=1(MONDAY) AND VBAT=1
        WriteRTCByte(4, value4);            //DATE=28
        WriteRTCByte(5, value5);            //MONTH=2
        WriteRTCByte(6, value6);            //YEAR=11
        WriteRTCByte(0, value1);            //START RTC, SECOND=00               

        return RTC_NTP_TIME_UPDATE_REQUEST_SUCCESS;
    }            
}
        
static void writeRTCInitialTimeFeed()
{
    unsigned int intTimeWday = 0x5,
                 intTimeDay  = 0x6,
                 intTimeMonth = 0x7,
                 intTimeYear  = 0x21,
                 intTimeHr    = 0x23,
                 intTimeMin   = 0x55,
                 intTimeSec   = 0x3;

    rtc_timekeeping_s.time_wday  = (unsigned int)intTimeWday;
    rtc_timekeeping_s.time_day   = (unsigned int)intTimeDay;
    rtc_timekeeping_s.time_month = (unsigned int)intTimeMonth;
    rtc_timekeeping_s.time_year  = (unsigned int)intTimeYear;
    rtc_timekeeping_s.time_hr    = (unsigned int)intTimeHr;
    rtc_timekeeping_s.time_min   = (unsigned int)intTimeMin;
    rtc_timekeeping_s.time_sec   = (unsigned int )intTimeSec;
    
    uint8_t rtc_start_bit   = 0x80,
            rtc_stop_bit    = 0x0,
            rtc_osc_en_bit  = 0x20,
            rtc_vbat_en_bit = 0x08;

    //rtc second & start  bit will be in same address 0x0
    // consideration wday_set_address contain VBAT enable bit & osc enable
    
    uint8_t rtc_start_second_set_address = 0,          
            rtc_min_set_address   = 1,
            rtc_hr_set_address    = 2,
            rtc_wday_set_address  = 3,
            rtc_day_set_address   = 4,
            rtc_month_set_address = 5,
            rtc_year_set_address  = 6;
    
    uint8_t rtc_min_val_str    = rtc_timekeeping_s.time_min,
            rtc_hr_val_str     = rtc_timekeeping_s.time_hr,
            rtc_day_val_str    = rtc_timekeeping_s.time_day,
            rtc_month_val_str  = rtc_timekeeping_s.time_month,
            rtc_year_val_str   = rtc_timekeeping_s.time_year,
            rtc_second_val_str = rtc_start_bit | rtc_timekeeping_s.time_sec,
            rtc_wday_val_str   = 0x0;

            rtc_wday_val_str |= rtc_osc_en_bit;
            rtc_wday_val_str |= (rtc_vbat_en_bit | rtc_timekeeping_s.time_wday);

    if(verbose == 1)
    {
            Serial.printf("--------------------------------------------------------");
            Serial.printf("WriteRTCByte address and values ... \n");
            Serial.printf("<0> address/value: %x/%d, %x/%d \n", rtc_start_second_set_address,
                                                                rtc_start_second_set_address,
                                                                rtc_stop_bit,
                                                                rtc_stop_bit);

            Serial.printf("<1> address/value: %x/%d, %x/%d \n", rtc_min_set_address,
                                                                rtc_min_set_address,
                                                                rtc_min_val_str,
                                                                rtc_min_val_str);
                                                                
            Serial.printf("<2> address/value: %x/%d, %x/%d \n", rtc_hr_set_address,
                                                                rtc_hr_set_address,
                                                                rtc_hr_val_str,
                                                                rtc_hr_val_str);
                                                                
            Serial.printf("<3> address/value: %x/%d, %x/%d \n", rtc_wday_set_address,
                                                                rtc_wday_set_address,
                                                                rtc_wday_val_str,
                                                                rtc_wday_val_str);
                                                                
            Serial.printf("<4> address/value: %x/%d, %x/%d \n", rtc_day_set_address,
                                                                rtc_day_set_address,
                                                                rtc_day_val_str,
                                                                rtc_day_val_str);
                                                                
            Serial.printf("<5> address/value: %x/%d, %x/%d \n", rtc_month_set_address,
                                                                rtc_month_set_address,
                                                                rtc_month_val_str,
                                                                rtc_month_val_str);
                                                                
            Serial.printf("<6> address/value: %x/%d, %x/%d \n", rtc_year_set_address,
                                                                rtc_year_set_address,
                                                                rtc_year_val_str,
                                                                rtc_year_val_str);

            Serial.printf("<0/7> address/value: %x/%d, %x/%d \n", rtc_start_second_set_address,
                                                                  rtc_start_second_set_address,
                                                                  rtc_second_val_str,
                                                                  rtc_second_val_str);
    }
    
            WriteRTCByte(rtc_start_second_set_address, rtc_stop_bit);
            WriteRTCByte(rtc_min_set_address, rtc_min_val_str);
            WriteRTCByte(rtc_hr_set_address, rtc_hr_val_str);
            WriteRTCByte(rtc_wday_set_address, rtc_wday_val_str);
            WriteRTCByte(rtc_day_set_address, rtc_day_val_str);
            WriteRTCByte(rtc_month_set_address, rtc_month_val_str);
            WriteRTCByte(rtc_year_set_address, rtc_year_val_str);
            WriteRTCByte(rtc_start_second_set_address, (unsigned char)rtc_second_val_str);
}

static void writeRTCFieldClear()
{
    WriteRTCByte(0,0);       //STOP RTC
    WriteRTCByte(1,0x0);    //MINUTE=18
    WriteRTCByte(2,0x0);    //HOUR=8
    WriteRTCByte(3,0x0);    //DAY=1(MONDAY) AND VBAT=1
    WriteRTCByte(4,0x0);    //DATE=28
    WriteRTCByte(5,0x0);    //MONTH=2
    WriteRTCByte(6,0x0);    //YEAR=11
    WriteRTCByte(0,0x80);    //START RTC, SECOND=00
}

// Called to read bytes from RTC
unsigned char ReadRTCByte(const unsigned char adr)
{
    unsigned char dataReadRTCByte;
    Wire.beginTransmission(0x6f);
    Wire.write(adr);
    Wire.endTransmission(); 
    /* Wire.requestFrom(2, 6);    // request 6 bytes from slave device #2 */
    Wire.requestFrom(0x6f,1);
    while (Wire.available()) dataReadRTCByte =Wire.read();
    return dataReadRTCByte;
}

// Called to write bytes to RTC
void WriteRTCByte(const unsigned char adr, const unsigned char data)
{
    Wire.beginTransmission(0x6f);
    Wire.write(adr);
    Wire.write(data);
    Wire.endTransmission();
} 

// Return the time as a string in format hh:mm:ss
String rtcTime() 
{ 
    //rtc_timezone_s
  
    data = ReadRTCByte(3);
    int week = data & 0xff >> (5);
    if(verbose == 1)
        Serial.printf("Week: /%x/%d \n", week, week);
        
    //rtc_timezone_s.time_wday =  week;
    rtc_timezone_s.time_wday = week;
    
    data=ReadRTCByte(2);
    int hour=data & 0xff>>(2);
    //rtc_timezone_s.time_hr = hour;
    rtc_timezone_s.time_hr = intToHex(hour);
    
    data=ReadRTCByte(1);
    int minute=data & 0xff>>(1);
    //rtc_timezone_s.time_min = minute;
    rtc_timezone_s.time_min = intToHex(minute);
    
    data=ReadRTCByte(0); 
    int second=data & 0xff>>(1);
    //rtc_timezone_s.time_sec = second;
    rtc_timezone_s.time_sec = intToHex(second);
   
    String rtcTime; 
    if (hour < 10){rtcTime += "0";} 
    rtcTime += String(hour,HEX);
    rtcTime += ":";  
    if (minute < 10){rtcTime += "0";}
    rtcTime += String(minute,HEX);  
    rtcTime += ":";  
    if (second < 10){rtcTime += "0";} 
    rtcTime += String(second,HEX);
    return rtcTime;  
}

// Return the date as a string in format dd:mm:yy
String rtcDate() 
{ 
    data=ReadRTCByte(4); 
    int day=data & 0xff>>(2);
    // rtc_timezone_s.time_day = day;
     rtc_timezone_s.time_day = intToHex(day);
    
    data=ReadRTCByte(5);
    int month=data & 0xff>>(3);
    //rtc_timezone_s.time_month = month;
    rtc_timezone_s.time_month = intToHex(month);
    
    data=ReadRTCByte(6);
    int year=data & 0xff>>(0);
   // rtc_timezone_s.time_year = year;
    rtc_timezone_s.time_year = intToHex(year);
    
    String rtcDate;
    rtcDate += String(day,HEX);
    rtcDate += ":";  
    rtcDate += String(month,HEX);  
    rtcDate += ":";  
    rtcDate += String(year,HEX);
    return rtcDate;  
}

void eepromRtcMemClear()
{
    EEPROM.write(RTC_ESP_EEPROM_FLASH_ADDRESS, 0);
    EEPROM.commit();
    //ESP.restart();
}

void eepromRtcSetupSupport()
{
    Wire.begin(13,16);

    EEPROM.begin(EEPROM_SIZE);
    
    eepromDataVal = EEPROM.read(0);
    if(verbose == 1)
    {
        Serial.printf("Value in address-0 <%d> \n", eepromDataVal);
    }
    delay(10);

    rtcFlashSts = EEPROM.read(RTC_ESP_EEPROM_FLASH_ADDRESS);
    if(verbose == 1)
    {
        Serial.printf("rtcFlshSts in %d/0x%x: <%d> \n", RTC_ESP_EEPROM_FLASH_ADDRESS, RTC_ESP_EEPROM_FLASH_ADDRESS, rtcFlashSts);
    }
    if(rtcFlashSts == 0)
    {
        if(verbose == 1)
        {
            Serial.printf("RTC not yet flashed... ready to flash \n");
        }
        EEPROM.write(RTC_ESP_EEPROM_FLASH_ADDRESS, RTC_ID_FIRST_TIME);
        EEPROM.commit();
     
        rtcIdFirstTimeSts = EEPROM.read(RTC_ESP_EEPROM_FLASH_ADDRESS);

        if(verbose == 1)
        {
            Serial.printf("<1> RTC_ESP_EEPROM_FLASH_ADDRESS: %d, rtcIdFirstTimeSts: %d, RTC_ID_FIRST_TIME: %d \n", RTC_ESP_EEPROM_FLASH_ADDRESS, rtcIdFirstTimeSts, RTC_ID_FIRST_TIME);
        }
        if(rtcIdFirstTimeSts == RTC_ID_FIRST_TIME)
        {   
            if(verbose == 1)
            {
                Serial.printf("First time excuted ... \n");
            }
            writeRTCInitialTimeFeed();
        }

        if(verbose == 1)
        {
            Serial.printf("RTC flash is done, gng to flash RTC_ID_NON_FLASH_TIME in address<%d/0x%x> \n", RTC_ESP_EEPROM_FLASH_ADDRESS,RTC_ESP_EEPROM_FLASH_ADDRESS);
        }
        EEPROM.write(RTC_ESP_EEPROM_FLASH_ADDRESS, RTC_ID_NON_FIRST_TIME);
        EEPROM.commit();
        rtcIdNonFirstTimeSts = EEPROM.read(RTC_ESP_EEPROM_FLASH_ADDRESS);
        if(verbose == 1)
        {
            Serial.printf("<2> RTC_ESP_EEPROM_FLASH_ADDRESS: %d, rtcIdNonFirstTimeSts: %d, RTC_ID_NON_FIRST_TIME: %d \n", RTC_ESP_EEPROM_FLASH_ADDRESS, rtcIdNonFirstTimeSts, RTC_ID_NON_FIRST_TIME);        
        }
        if(rtcIdNonFirstTimeSts == RTC_ID_NON_FIRST_TIME)
        {
            if(verbose == 1)
            {
                Serial.printf("RTC Flash initial commit success ... \n");
            }
        }
    }
    
//RTC_EEPROM_MEM_CLEAR
#ifdef RTC_EEPROM_MEM_CLEAR
        EEPROM.write(RTC_ESP_EEPROM_FLASH_ADDRESS, 0);
        EEPROM.commit();
#endif
}

void rtcPrintInLoop()
{
    if(verbose == 1)
    {
        Serial.printf("\n---------------------------------------------------------------------------------\n");
        Serial.printf("rtcPrintInLoop Excutes \n");
    }
    /* uncomment to printout the debug's and run on seperate task for accurate time perdication.
    Serial.print(rtcTime());
    Serial.println();
    //Print the date
    Serial.print(rtcDate());
    Serial.println();

    if(verbose == 1)
    {
        Serial.printf("Complete Time: ");
    }
    Serial.print(rtcTime());
    if(verbose == 1)
    {
        Serial.printf(",Date: ");
    }
    Serial.print(rtcDate());
    Serial.printf("\n");
    delay(100);
    */
    rtcTime();
    rtcDate();
    if(verbose == 1)
    { 
        Serial.printf("----------------------------------------------------------------------------------\n");
    }
}
