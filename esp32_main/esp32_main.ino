
#include<stdio.h>
#include<stdlib.h>

#include "freertos/task.h"
#include "i2c_util.h"
#include "wifi_util.h"
#include "rtc_mcp79410.h"
#include "ntp_time_support.h"
#include "EEPROM.h"
#include "errorkeys.h"
#include "common_util.h"
#include "rtc_time_sync.h"

/*
 * RTC_timeKeepingSyncTask(void *param_p) intially this task will sync with NTP time using WiFi
 * infuture:
 * 1. Use NTP timekeeping using GPRS                                          - Priority level(1) - Low
 * 2. Use GPS time while network is not there                                 - Priority Level(2) - Medium
 * 3. Use SIM868 time to keep while having good/very good network connections - Priority Level(3) - High
 */
             
static void disp_buf(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        Serial.printf("%02x ", buf[i]);
        if ((i + 1) % 16 == 0) {
            Serial.printf("\n");
        }
    }
    Serial.printf("\n");
}

void setup()
{
    Serial.begin(115200);
    
    WIFI_connectFun();
    ntpInitGetTime();
    eepromRtcSetupSupport();
    WIFI_disconnectFun();
 
    xTaskCreatePinnedToCore(RTC_timeKeepingSyncTask, "RTC-Time_Keeping_Task", 1024*3, NULL , 1, NULL, 1);

    ESP_ERROR_CHECK(I2C_slaveInit());

    xTaskCreate(I2C_slaveRead, "I2C - Slave read", 1024*3, NULL, 1, NULL);
    if(verbose == 1)
    {    
        Serial.printf("Setup done ... \n");  
    }
    delay(100);
}

void loop(){
    rtcPrintInLoop();
    printLocalTime();

    //delay(1000);
    if(verbose == 1)
    {
        Serial.printf("%s> Key status %d, %d-%d-%d %d:%d:%d \n", __FUNCTION__,
                                                                 rtc_ntp_match_keys.time_wday_key,
                                                                 rtc_ntp_match_keys.time_day_key,
                                                                 rtc_ntp_match_keys.time_month_key,
                                                                 rtc_ntp_match_keys.time_year_key,
                                                                 rtc_ntp_match_keys.time_hr_key,
                                                                 rtc_ntp_match_keys.time_min_key,
                                                                 rtc_ntp_match_keys.time_sec_key);    
    }
    delay(100);
                                                             
}
