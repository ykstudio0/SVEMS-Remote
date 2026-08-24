//-------------------------------------------------------------
// File : WiFiService.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-17
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Remote Display Wifi Manage
//-------------------------------------------------------------

#include "WiFiService.h"
#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>

namespace
{
    WebServer SetupServer(80);
    
    constexpr uint32_t RETRY_INTERVAL_MS =
        5000UL;

    uint32_t lastRetry =
        0;

    bool LoadWiFiConfig(
        String& ssid,
        String& password)
    {
        Preferences preferences;

        if (!preferences.begin(
                "wifi",
                false))
        {
            return false;
        }

        ssid =
            preferences.getString(
                "ssid",
                ""
            );

        password =
            preferences.getString(
                "password",
                ""
            );

        preferences.end();

        if (ssid.length() == 0U)
        {
            Serial.println(
                "[WIFI] No saved config"
            );

            return false;
        }

        Serial.println(
            "[WIFI] Using saved config"
        );

        return true;
    }
}

void WiFiService::Begin()
{
    String ssid;
    String password;

    if (!LoadWiFiConfig(
            ssid,
            password))
    {
        Serial.println(
            "[WIFI] Starting setup mode"
        );

        StartSetupMode();

        return;
    }

    WiFi.mode(
        WIFI_STA
    );

    WiFi.begin(
        ssid.c_str(),
        password.c_str()
    );

    Serial.println(
        "[WIFI] Connecting..."
    );
}

void WiFiService::Update()
{
    const wifi_mode_t mode =
        WiFi.getMode();

    if (mode == WIFI_AP ||
        mode == WIFI_AP_STA)
    {
        SetupServer.handleClient();
        return;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    const uint32_t now =
        millis();

    if (now - lastRetry <
            RETRY_INTERVAL_MS)
    {
        return;
    }

    lastRetry =
        now;

    Serial.println(
        "[WIFI] Retry..."
    );

    String ssid;
    String password;

    if (!LoadWiFiConfig(
            ssid,
            password))
    {
        StartSetupMode();
        return;
    }

    WiFi.disconnect();

    WiFi.begin(
        ssid.c_str(),
        password.c_str()
    );
}

bool WiFiService::IsConnected()
{
    return
        WiFi.status() ==
        WL_CONNECTED;
}

bool WiFiService::SaveConfig(
    const String& ssid,
    const String& password)
{
    if (ssid.length() == 0U)
    {
        return false;
    }

    Preferences preferences;

    if (!preferences.begin(
            "wifi",
            false))
    {
        return false;
    }

    const size_t ssidWritten =
        preferences.putString(
            "ssid",
            ssid
        );

    preferences.putString(
        "password",
        password
    );

    preferences.end();

    return ssidWritten > 0U;
}

bool WiFiService::StartSetupMode()
{
    WiFi.disconnect(
        true,
        false
    );

    WiFi.mode(
        WIFI_AP
    );

    if (!WiFi.softAP(
            "SVEMS-SETUP"))
    {
        Serial.println(
            "[WIFI] Setup AP failed"
        );

        return false;
    }

    Serial.println(
        "[WIFI] Setup AP started"
    );

    SetupServer.on(
        "/",
        HTTP_GET,
        []()
        {
            const char* html =
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name='viewport' "
                "content='width=device-width, initial-scale=1'>"
                "<style>"
                "body{font-family:Arial,sans-serif;padding:24px;}"
                "input{font-size:20px;width:100%;padding:10px;"
                "margin:8px 0 20px 0;box-sizing:border-box;}"
                "button{font-size:22px;padding:12px 24px;}"
                "label{font-size:22px;}"
                "</style>"
                "</head>"
                "<body>"
                "<h2>SVEMS WiFi Setup</h2>"
                "<form method='POST' action='/save'>"
                "<label>SSID</label>"
                "<input name='ssid' type='text'>"
                "<label>Password</label>"
                "<input name='password' type='password'>"
                "<button type='submit'>Save</button>"
                "</form>"
                "</body>"
                "</html>";

            SetupServer.send(
                200,
                "text/html",
                html
            );
        }
    );

    SetupServer.on(
        "/save",
        HTTP_POST,
        []()
        {
            const String ssid =
                SetupServer.arg(
                    "ssid"
                );

            const String password =
                SetupServer.arg(
                    "password"
                );

            if (ssid.length() == 0U)
            {
                SetupServer.send(
                    400,
                    "text/plain",
                    "SSID required"
                );

                return;
            }

            if (!WiFiService::SaveConfig(
                    ssid,
                    password))
            {
                SetupServer.send(
                    500,
                    "text/plain",
                    "Save failed"
                );

                return;
            }

            SetupServer.send(
                200,
                "text/html",
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name='viewport' "
                "content='width=device-width, initial-scale=1'>"
                "<style>"
                "body{font-family:Arial,sans-serif;"
                "padding:24px;font-size:22px;}"
                "h2{font-size:36px;margin-bottom:24px;}"
                "p{font-size:24px;line-height:1.5;}"
                "</style>"
                "</head>"
                "<body>"
                "<h2>Saved</h2>"
                "<p>WiFi configuration saved.</p>"
                "<p>SVEMS will restart...</p>"
                "</body>"
                "</html>"
            );

            delay(
                1000
            );

            ESP.restart();
        }
    );

    SetupServer.onNotFound(
        []()
        {
            SetupServer.send(
                404,
                "text/plain",
                "Not Found"
            );
        }
    );

    SetupServer.begin();

    Serial.println(
        "[WIFI] Setup WebServer started"
    );

    return true;
}

bool WiFiService::IsSetupMode()
{
    const wifi_mode_t mode =
        WiFi.getMode();

    return
        mode == WIFI_AP ||
        mode == WIFI_AP_STA;
}