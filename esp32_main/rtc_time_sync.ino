#include "rtc_time_sync.h"
#include "common_util.h"

static void RTC_timeKeepingSyncTask(void *param_p)
{
    int ret = 0;
    if(verbose == 1)
    {
        Serial.printf("RTC_timeKeepingSyncTask \n");
    }
    bool first_time_rtc_ntp_sync = true;
   
    rtc_ntp_match_keys.time_day_key = 0;
    rtc_ntp_match_keys.time_month_key = 0;
    rtc_ntp_match_keys.time_year_key = 0;
    rtc_ntp_match_keys.time_hr_key = 0;
    rtc_ntp_match_keys.time_min_key = 0;
    rtc_ntp_match_keys.time_sec_key = 0;
    rtc_ntp_match_keys.time_wday_key = 0;
    
    while(1)
    {

         Serial.printf("%s> NTP: %02d, %02d - %02d - %02d  %02d : %02d : %02d \n", __FUNCTION__,
                                                                                   ntp_timezone_s.time_wday,                                                                                                        
                                                                                   ntp_timezone_s.time_day, 
                                                                                   ntp_timezone_s.time_month,
                                                                                   ntp_timezone_s.time_year - 2000,                       
                                                                                   ntp_timezone_s.time_hr,
                                                                                   ntp_timezone_s.time_min,
                                                                                   ntp_timezone_s.time_sec);
                                                                          
         Serial.printf("%s> RTC: %02d, %02d - %02d - %02d  %02d : %02d : %02d  \n", __FUNCTION__,
                                                                                    rtc_timezone_s.time_wday,
                                                                                    rtc_timezone_s.time_day,
                                                                                    rtc_timezone_s.time_month,
                                                                                    rtc_timezone_s.time_year,
                                                                                    rtc_timezone_s.time_hr,
                                                                                    rtc_timezone_s.time_min,
                                                                                    rtc_timezone_s.time_sec);
          Serial.printf("\n--------------------------------------------------------------------------------\n");
          
          if((unsigned int)ntp_timezone_s.time_wday == (unsigned int)rtc_timezone_s.time_wday)
          {
              rtc_ntp_match_keys.time_wday_key = 1;
              if(verbose == 1)
              {  
                  Serial.printf("%s> Weekday matched ... \n", __FUNCTION__);
              }
          }
          else
          {
              rtc_ntp_match_keys.time_wday_key = 0;
          }
          
          if((unsigned int)ntp_timezone_s.time_day == (unsigned int)rtc_timezone_s.time_day)
          {
              rtc_ntp_match_keys.time_day_key = 1;
              if(verbose == 1)
              {
                  Serial.printf("%s> Day matched ... \n", __FUNCTION__);
              }
          }
          else
          {
              rtc_ntp_match_keys.time_day_key = 0;
          }
          
          if((unsigned int)ntp_timezone_s.time_month == (unsigned int)rtc_timezone_s.time_month)
          {
              rtc_ntp_match_keys.time_month_key = 1;
              if(verbose == 1)
              {
                  Serial.printf("%s> Month matched ... \n", __FUNCTION__);
              }
          }
          else
          {
              rtc_ntp_match_keys.time_month_key = 0;
          }
          
          if((unsigned int)ntp_timezone_s.time_year - 2000 == (unsigned int)rtc_timezone_s.time_year)
          {
              rtc_ntp_match_keys.time_year_key = 1;
              if(verbose == 1)
              {
                  Serial.printf("%s> Year matched ... \n", __FUNCTION__);
              }
          }
          else
          {
              rtc_ntp_match_keys.time_year_key = 0;
          }
          
          if((unsigned int)ntp_timezone_s.time_hr == (unsigned int)rtc_timezone_s.time_hr)
          {
              rtc_ntp_match_keys.time_hr_key = 1;
              if(verbose == 1)
              {
                  Serial.printf("%s> Hour matched ... \n", __FUNCTION__);
              }
          }
          else
          {
              rtc_ntp_match_keys.time_hr_key = 0;
          }
          
          if((unsigned int)ntp_timezone_s.time_min == (unsigned int)rtc_timezone_s.time_min)
          {
              rtc_ntp_match_keys.time_min_key = 1;
              if(verbose == 1)
              {
                  Serial.printf("%s> Minute matched ... \n", __FUNCTION__);
              }
          }
          else
          {
              rtc_ntp_match_keys.time_min_key = 0;
          }
          
          if((unsigned int)ntp_timezone_s.time_sec == (unsigned int)rtc_timezone_s.time_sec)
          {
              rtc_ntp_match_keys.time_sec_key = 1;
              if(verbose == 1)
              {
                  Serial.printf("%s> Seconds matched ... \n", __FUNCTION__);
              }
          }
          else
          {
              rtc_ntp_match_keys.time_sec_key = 0;
          }
          if(verbose == 1)
          {
              Serial.printf("\n-------------------------------------------------------------------------------\n");
          }

          if(rtc_ntp_match_keys.time_wday_key == 1 && 
             rtc_ntp_match_keys.time_day_key == 1 &&
             rtc_ntp_match_keys.time_month_key == 1 && 
             rtc_ntp_match_keys.time_year_key == 1 &&
             rtc_ntp_match_keys.time_hr_key == 1 &&
             rtc_ntp_match_keys.time_min_key == 1 &&
             rtc_ntp_match_keys.time_sec_key == 1)
             {
                  if(verbose == 1 || verbose == 0)
                  {
                      Serial.printf("\n RTC & NTP Complete Timekeeping keys matched ... \n");
                  }
             }
             
            if(rtc_ntp_match_keys.time_hr_key != 1 || rtc_ntp_match_keys.time_day_key != 1)
            {
                  rtc_ntp_unknown_miss_match_cnt ++;
                  if(verbose == 1)
                  {
                      Serial.printf("\n RTC Timekeeping hr/day key not matched <%lu>\n", rtc_ntp_unknown_miss_match_cnt);
                  } 
            }


          if(first_time_rtc_ntp_sync == false)
          {
            
            if(rtc_ntp_match_keys.time_wday_key != 1 || rtc_ntp_match_keys.time_day_key != 1 || rtc_ntp_match_keys.time_month_key != 1 || rtc_ntp_match_keys.time_year_key != 1 || rtc_ntp_match_keys.time_hr_key != 1 || rtc_ntp_match_keys.time_min_key != 1)
            //if(rtc_ntp_match_keys.time_wday_key != 1 || rtc_ntp_match_keys.time_day_key != 1 || rtc_ntp_match_keys.time_month_key != 1 || rtc_ntp_match_keys.time_year_key != 1 || rtc_ntp_match_keys.time_hr_key != 1) 
            {
                 
                 rtc_ntp_time_miss_match_cnt ++;
                 if(verbose == 1)
                 {              
                      Serial.printf("\n RTC Timekeeping Keys Not matched <%lu>... \n", rtc_ntp_time_miss_match_cnt);
                      Serial.printf("%s> RTC Timekeeping Keystore: %d, %02d-%02d-%02d %02d:%02d:%02d \n", __FUNCTION__,
                                                                                                          rtc_ntp_match_keys.time_wday_key,
                                                                                                          rtc_ntp_match_keys.time_day_key,
                                                                                                          rtc_ntp_match_keys.time_month_key,
                                                                                                          rtc_ntp_match_keys.time_year_key,
                                                                                                          rtc_ntp_match_keys.time_hr_key,
                                                                                                          rtc_ntp_match_keys.time_min_key,
                                                                                                          rtc_ntp_match_keys.time_sec_key);

                          RTC_NTP_TIME_UPDATE_REQUEST = true;         
                          ret = writeTimeRtcNtpSync();                                                                      //instead of passing directly update the value ...
                          if(ret == RTC_NTP_TIME_UPDATE_REQUEST_FAILED)
                          {
                              Serial.printf("%s> RTC Ntp Time Update Request Failed !!! \n", __FUNCTION__);   
                              RTC_NTP_TIME_UPDATE_REQUEST = false;                                                          
                          }
                          else
                          {
                              Serial.printf("%s> RTC Ntp Time Update Request Success ... \n", __FUNCTION__);
                              RTC_NTP_TIME_UPDATE_REQUEST = false;
                          }
                 }
            }
          } //first_time_rtc_ntp_sync firs ttime the above block will not be executed only 2nd time will execute.
          first_time_rtc_ntp_sync = false;
          vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    vTaskDelete(NULL);
}
