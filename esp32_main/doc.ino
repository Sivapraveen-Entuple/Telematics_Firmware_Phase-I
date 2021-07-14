
#if 0

RTC Points and Todo
-> Added rtc, eeprom - done
-> added ntp support - done
-> error checks - done
-> time sync with ntp - todo
-> check if, eeprom clear memory needs restart enable ESP.restart inside the void eepromRtcMemClear();
-> add error checks in all functions
-> remove while loop for ntp support and create a task and keep on trying
-> in future keep on trying to connect wifi should not block, wifi should be used only for test local otherthen that need to use GPRS support to fetch


Important Supportes for all component
-> add all the processing task in core0, and network support task core1

/*
 * RTC: Supported protect features
 * -> N number flash without rewriting timing for EEPROM (EEPROM write protect)
 *    -> Dedicated EEPROM register address 0(zero) - size 1 for monitoring the first time and non first time status
 *    -> Simple change and flash will not rewrite the time/date
 *    -> To write/change the values into the register, needs to enable #define RTC_EEPROM_MEM_CLEAR in rtc_mcp79410.h file  
 *    -> continuously monitor the time with live time from NTP -or- GSM -or- GPS, and synchronize the data(write) those to the internal register fields inside the RTC(mcp79410)  - todo
 *    -> Enable and Disable for all features onpermise                                                                                                                            - todo
 *    -> Enable and Disable support over cloud (via mqtt(test phase) -or- websockets -or- TBD(communication support) affect on global variable or use quque)                      - todo
 */


Task and Queues
-> Add task_idx for all the processing task to monitor task, unique task ID

#endif
