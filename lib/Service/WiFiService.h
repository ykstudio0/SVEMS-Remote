//-------------------------------------------------------------
// File : WiFiService.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-17
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Remote Display Wifi Manage
//-------------------------------------------------------------

#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>

class WiFiService
{
public:
    static void Begin();

    static void Update();

    static bool IsConnected();

    static bool SaveConfig(
        const String& ssid,
        const String& password
    );

    static bool StartSetupMode();

    static bool IsSetupMode();

    static void StopSetupMode();
};

#endif