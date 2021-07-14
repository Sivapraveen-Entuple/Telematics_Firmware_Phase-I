#ifndef __NTP_TIME_SUPPORT_H__
#define __NTP_TIME_SUPPORT_H__

#include "wifi_util.h"
#include "time.h"
#include "rtc_time_sync.h"
#include "common_util.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

int time_wday,
    time_hr,
    time_min,
    time_sec,
    time_day,
    time_month,
    time_year;

void printLocalTime();
void ntpInitGetTime();


void printLocalTime()
{
    if(verbose == 1)
    {
        Serial.printf("\n---------------------------------------------------------------------------------\n");
        Serial.printf("PrintLocalTime exucutes \n");
    }
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
    }
    
    ntp_timezone_s.time_wday = timeinfo.tm_wday;
    ntp_timezone_s.time_hr = timeinfo.tm_hour;
    ntp_timezone_s.time_min = timeinfo.tm_min;
    ntp_timezone_s.time_sec = timeinfo.tm_sec;
    ntp_timezone_s.time_day = timeinfo.tm_mday;
    ntp_timezone_s.time_month = (timeinfo.tm_mon) + 1;
    ntp_timezone_s.time_year = (timeinfo.tm_year) + 1900;

    
    if(verbose == 1)
    {
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    }
    
    if(verbose == 1)
    {
        Serial.printf("\nNow is : %d %d-%02d-%02d %02d:%02d:%02d\n",
                                                                    timeinfo.tm_wday, 
                                                                    (timeinfo.tm_year) + 1900,
                                                                    (timeinfo.tm_mon) + 1,
                                                                    timeinfo.tm_mday, 
                                                                    timeinfo.tm_hour,
                                                                    timeinfo.tm_min,
                                                                    timeinfo.tm_sec);
    }
    time_wday = timeinfo.tm_wday;
    time_hr = timeinfo.tm_hour;
    time_min = timeinfo.tm_min;
    time_sec = timeinfo.tm_sec;
    time_day = timeinfo.tm_mday;
    time_month = (timeinfo.tm_mon) + 1;
    time_year = (timeinfo.tm_year) + 1900;

    if(verbose == 1)
    {
        switch(time_wday)
        {
            case 0:
                Serial.printf("Weekday: Sunday \n");
                break;
            case 1:
                Serial.printf("Weekday: Monday \n");
                break;
            case 2:
                Serial.printf("Weekday: Tuesday \n");
                break;
            case 3:
                Serial.printf("Weekday: Wednesday \n");
                break;
            case 4:
                Serial.printf("Weekday: Thurshday \n");
                break;
            case 5:
                Serial.printf("Weekday: Friday \n");
                break;
            case 6:
                Serial.printf("Weekday: Saturday \n");
                break;
        }
        Serial.printf("Wday: %02d \n", time_wday);
        Serial.printf("Hour: %02d \n", time_hr);
        Serial.printf("Min: %02d \n", time_min);
        Serial.printf("Second: %02d \n", time_sec);
        Serial.printf("Day: %02d \n", time_day);
        Serial.printf("Month: %02d \n", time_month);
        Serial.printf("Year: %02d \n", time_year);
        Serial.printf("\n---------------------------------------------------------------------------------\n");
    }
}

void ntpInitGetTime()
{
    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
}

#endif
