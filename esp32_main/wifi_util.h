#ifndef __WIFI_UTIL__
#define __WIFI_UTIL__

#include <WiFi.h>
#include "common_util.h"

/*
 * WiFi Status codes
 * 
 * > (3) WL_CONNECTED: assigned when connected to a WiFi network;
 * > (255) WL_NO_SHIELD: assigned when no WiFi shield is present;
 * > (?) WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called 
 *                       and remains active until the number of attempts expires (resulting 
 *                       in WL_CONNECT_FAILED) or a connection is established (resulting in 
 *                       WL_CONNECTED);
 * > (1) WL_NO_SSID_AVAIL: assigned when no SSID are available;
 * > (2) WL_SCAN_COMPLETED: assigned when the scan networks is completed;
 * > (4) WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
 * > (5) WL_CONNECTION_LOST: assigned when the connection is lost;
 * > (6) WL_DISCONNECTED: assigned when disconnected from a network;
 */

const char* ssid       = "sivapraveen";
const char* password   = "12345678";


static void WIFI_connectFun()
{
    //connect to WiFi
    if(verbose == 1)
    {
        Serial.printf("Connecting to %s ", ssid);
    }
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if(verbose == 1)
        {
            Serial.print(".");
        }
    }
    if(verbose == 1)
    {
        Serial.println(" CONNECTED");
    }
}

static void WIFI_disconnectFun()
{
    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

#endif
