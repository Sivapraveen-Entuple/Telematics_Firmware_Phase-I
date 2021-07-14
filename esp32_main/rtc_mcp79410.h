#ifndef __RTC_MCP79410_H__
#define __RTC_MCP79410_H__

#include <Wire.h>
unsigned char data;

#define RTC_ESP_EEPROM_FLASH_ADDRESS 0  
#define EEPROM_SIZE 1

#define RTC_ID_FIRST_TIME 14
#define RTC_ID_NON_FIRST_TIME 15
#define RTC_ID_CHANGE_NEEDED 16
#define RTC_ID_CHANGE_AFFECT_ACCEPTED 17
#define RTC_ID_CHANGE_AFFECT_NOT_ACCEPTED 18

bool RTC_NTP_TIME_UPDATE_REQUEST = false;                      //request flag to update the NTP live time to RTC
#define RTC_NTP_TIME_UPDATE_REQUEST_FAILED -1
#define RTC_NTP_TIME_UPDATE_REQUEST_SUCCESS 1


int eepromDataVal = 0;
int rtcFlashSts = 0;
int rtcIdFirstTimeSts = 0,
    rtcIdNonFirstTimeSts = 0;

/* clearing RTC_EEPROM_MEM_CLEAR Enable only when customistion needed */
//#define RTC_EEPROM_MEM_CLEAR     

static void writeRTCInitialTimeFeed();
unsigned char ReadRTCByte(const unsigned char adr);
void WriteRTCByte(const unsigned char adr, const unsigned char data);
int writeTimeRtcNtpSync();
String rtcTime();
String rtcDate();
void eepromRtcSetupSupport();
static void writeRTCFieldClear();
void eepromRtcMemClear();

#endif
