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
    struct WiFiProfile
    {
        String ssid;
        String password;
        bool enabled = false;
    };

    static constexpr uint8_t
        MAX_PROFILES = 3U;

    static void Begin();

    static void Update();

    static bool IsConnected();

    static bool SaveConfig(
        const String& ssid,
        const String& password
    );

    static bool SaveProfile(
        uint8_t index,
        const String& ssid,
        const String& password,
        bool enabled = true
    );

    static bool StartSetupMode();

    static bool IsSetupMode();

    static void StopSetupMode();

private:
    static bool LoadProfile(
        uint8_t index,
        WiFiProfile& profile
    );

    static bool TryConnectProfiles();

    static uint8_t CurrentProfile;

    static bool Connecting;

    static uint32_t ConnectStartMs;

    static uint32_t LastRetryMs;

    static constexpr uint32_t
        CONNECT_TIMEOUT_MS =
            15000UL;

    static constexpr uint32_t
        RETRY_INTERVAL_MS =
            5000UL;

    static bool StartNextProfile();
};

#endif