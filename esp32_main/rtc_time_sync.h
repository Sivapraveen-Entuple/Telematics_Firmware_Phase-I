#ifndef __RTC_TIME_SYNC_H__
#define __RTC_TIME_SYNC_H__

struct __timezone__ { 
    unsigned int time_wday;
    unsigned int time_hr;
    unsigned int time_min;
    unsigned int time_sec;
    unsigned int time_day;
    unsigned int time_month;
    unsigned int time_year;
};

struct __timezone_match_key__ {
    uint8_t time_wday_key:2;
    uint8_t time_hr_key:2;
    uint8_t time_min_key:2;
    uint8_t time_sec_key:2;
    uint8_t time_day_key:2;
    uint8_t time_month_key:2;
    uint8_t time_year_key:2;
};

struct __timezone__ ntp_timezone_s;
struct __timezone__ rtc_timezone_s;
struct __timezone__ rtc_timekeeping_s;                                                // for intitial setup (WriteRTCByte)



struct __timezone_match_key__ rtc_ntp_match_keys;

unsigned long int rtc_ntp_unknown_miss_match_cnt = 0;
static unsigned long int rtc_ntp_time_miss_match_cnt = 0;

#endif
