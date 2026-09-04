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
}

uint8_t WiFiService::CurrentProfile =
        0U;

bool WiFiService::Connecting =
    false;

uint32_t WiFiService::ConnectStartMs =
    0U;

uint32_t WiFiService::LastRetryMs =
    0U;

bool WiFiService::StartNextProfile()
{
    while (
        CurrentProfile <
        MAX_PROFILES)
    {
        WiFiProfile profile;

        const uint8_t index =
            CurrentProfile++;

        if (!LoadProfile(
                index,
                profile))
        {
            continue;
        }

        Serial.print(
            "[WIFI] Trying profile "
        );

        Serial.print(
            index
        );

        Serial.print(
            ": "
        );

        Serial.println(
            profile.ssid
        );

        WiFi.disconnect();

        WiFi.begin(
            profile.ssid.c_str(),
            profile.password.c_str()
        );

        Connecting =
            true;

        ConnectStartMs =
            millis();

        return true;
    }

    Connecting =
        false;

    return false;
}

void WiFiService::Begin()
{
    WiFi.mode(
        WIFI_STA
    );

    CurrentProfile =
        0U;

    Connecting =
        false;

    LastRetryMs =
        millis();

    if (!StartNextProfile())
    {
        Serial.println(
            "[WIFI] No saved profiles"
        );

        StartSetupMode();
    }
}

void WiFiService::Update()
{
    const wifi_mode_t mode =
        WiFi.getMode();

    if (
        mode == WIFI_AP ||
        mode == WIFI_AP_STA
    )
    {
        SetupServer.handleClient();
        return;
    }

    //-------------------------------------------------
    // Connected
    //-------------------------------------------------

    if (
        WiFi.status() ==
        WL_CONNECTED
    )
    {
        if (Connecting)
        {
            Serial.print(
                "[WIFI] Connected: "
            );

            Serial.println(
                WiFi.SSID()
            );

            Serial.print(
                "[WIFI] IP: "
            );

            Serial.println(
                WiFi.localIP()
            );
        }

        Connecting =
            false;

        return;
    }

    const uint32_t now =
        millis();

    //-------------------------------------------------
    // Connecting
    //-------------------------------------------------

    if (Connecting)
    {
        if (
            now - ConnectStartMs <
            CONNECT_TIMEOUT_MS
        )
        {
            return;
        }

        Serial.println(
            "[WIFI] Profile timeout"
        );

        // 현재 Profile 연결 시도 종료
        WiFi.disconnect(
            false,
            false
        );

        Connecting =
            false;

        if (StartNextProfile())
        {
            return;
        }

        //-------------------------------------------------
        // All profiles failed
        //-------------------------------------------------

        CurrentProfile =
            0U;

        LastRetryMs =
            now;

        Serial.println(
            "[WIFI] All profiles failed"
        );

        return;
    }

    //-------------------------------------------------
    // Retry delay
    //-------------------------------------------------

    if (
        now - LastRetryMs <
        RETRY_INTERVAL_MS
    )
    {
        return;
    }

    LastRetryMs =
        now;

    CurrentProfile =
        0U;

    Serial.println(
        "[WIFI] Retry profiles"
    );

    StartNextProfile();
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
    return SaveProfile(
        0U,
        ssid,
        password,
        true
    );
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
            WiFiService::WiFiProfile p0;
            WiFiService::WiFiProfile p1;
            WiFiService::WiFiProfile p2;

            WiFiService::LoadProfile(
                0U,
                p0
            );

            WiFiService::LoadProfile(
                1U,
                p1
            );

            WiFiService::LoadProfile(
                2U,
                p2
            );

            String html;

            html.reserve(
                3000
            );

            html +=
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name='viewport' "
                "content='width=device-width, initial-scale=1'>"

                "<style>"

                "body{"
                "font-family:Arial,sans-serif;"
                "padding:12px;"
                "margin:0;"
                "}"

                "h2{"
                "font-size:24px;"
                "margin:4px 0 12px 0;"
                "}"

                "h3{"
                "font-size:17px;"
                "margin:10px 0 5px 0;"
                "}"

                "label{"
                "font-size:14px;"
                "display:block;"
                "margin:3px 0 2px 0;"
                "}"

                "input{"
                "font-size:15px;"
                "width:100%;"
                "height:34px;"
                "padding:4px 6px;"
                "margin:0 0 5px 0;"
                "box-sizing:border-box;"
                "}"

                "button{"
                "font-size:17px;"
                "width:100%;"
                "height:40px;"
                "margin-top:10px;"
                "}"

                "</style>"
                "</head>"

                "<body>"
                "<h2>SVEMS WiFi Setup</h2>"
                "<form method='POST' action='/save'>";

            //-------------------------------------------------
            // Profile 1
            //-------------------------------------------------

            html +=
                "<h3>Profile 1 : ";

            html +=
                p0.ssid.length() > 0U
                    ? p0.ssid
                    : "-";

            html +=
                "</h3>"
                "<label>SSID</label>"
                "<input name='ssid0' type='text'>"
                "<label>Password</label>"
                "<input name='pass0' type='password'>";

            //-------------------------------------------------
            // Profile 2
            //-------------------------------------------------

            html +=
                "<h3>Profile 2 : ";

            html +=
                p1.ssid.length() > 0U
                    ? p1.ssid
                    : "-";

            html +=
                "</h3>"
                "<label>SSID</label>"
                "<input name='ssid1' type='text'>"
                "<label>Password</label>"
                "<input name='pass1' type='password'>";

            //-------------------------------------------------
            // Profile 3
            //-------------------------------------------------

            html +=
                "<h3>Profile 3 : ";

            html +=
                p2.ssid.length() > 0U
                    ? p2.ssid
                    : "-";

            html +=
                "</h3>"
                "<label>SSID</label>"
                "<input name='ssid2' type='text'>"
                "<label>Password</label>"
                "<input name='pass2' type='password'>"

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
            bool savedAny =
                false;

            for (
                uint8_t index = 0U;
                index < WiFiService::MAX_PROFILES;
                ++index)
            {
                const String ssidName =
                    "ssid" + String(index);

                const String passName =
                    "pass" + String(index);

                const String ssid =
                    SetupServer.arg(
                        ssidName
                    );

                const String password =
                    SetupServer.arg(
                        passName
                    );

                if (
                    ssid.length() == 0U
                )
                {
                    continue;
                }

                if (
                    WiFiService::SaveProfile(
                        index,
                        ssid,
                        password,
                        true
                    )
                )
                {
                    savedAny =
                        true;
                }
            }

            if (!savedAny)
            {
                SetupServer.send(
                    400,
                    "text/plain",
                    "At least one SSID required"
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
                "</head>"
                "<body>"
                "<h2>Saved</h2>"
                "<p>WiFi profiles saved.</p>"
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

void WiFiService::StopSetupMode()
{
    //-------------------------------------------------
    // Setup WebServer 종료
    //-------------------------------------------------

    SetupServer.stop();

    //-------------------------------------------------
    // Setup AP 종료
    //-------------------------------------------------

    WiFi.softAPdisconnect(
        false
    );

    Serial.println(
        "[WIFI] Setup AP stopped"
    );

    //-------------------------------------------------
    // 기존 WiFi 설정으로 Station 복귀
    //-------------------------------------------------

    Begin();

    Serial.println(
        "[WIFI] Station recovery started"
    );
}

bool WiFiService::IsSetupMode()
{
    const wifi_mode_t mode =
        WiFi.getMode();

    return
        mode == WIFI_AP ||
        mode == WIFI_AP_STA;
}

bool WiFiService::SaveProfile(
    uint8_t index,
    const String& ssid,
    const String& password,
    bool enabled)
{
    if (
        index >= MAX_PROFILES ||
        ssid.length() == 0U
    )
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

    const String prefix =
        "p" + String(index);

    const size_t ssidWritten =
        preferences.putString(
            (prefix + "ssid").c_str(),
            ssid
        );

    preferences.putString(
        (prefix + "pass").c_str(),
        password
    );

    preferences.putBool(
        (prefix + "en").c_str(),
        enabled
    );

    preferences.end();


    preferences.putString(
        (prefix + "pass").c_str(),
        password
    );

    preferences.putBool(
        (prefix + "en").c_str(),
        enabled
    );

    preferences.end();

    Serial.print(
        "[WIFI] Profile saved "
    );

    Serial.print(
        index
    );

    Serial.print(
        ": "
    );

    Serial.println(
        ssid
    );


    return ssidWritten > 0U;
}

bool WiFiService::LoadProfile(
    uint8_t index,
    WiFiProfile& profile)
{
    if (index >= MAX_PROFILES)
    {
        return false;
    }

    Preferences preferences;

    if (!preferences.begin(
            "wifi",
            true))
    {
        return false;
    }

    const String prefix =
        "p" + String(index);

    const String ssidKey =
        prefix + "ssid";

    if (!preferences.isKey(
            ssidKey.c_str()))
    {
        preferences.end();

        return false;
    }

    profile.ssid =
        preferences.getString(
            (prefix + "ssid").c_str(),
            ""
        );

    profile.password =
        preferences.getString(
            (prefix + "pass").c_str(),
            ""
        );

    profile.enabled =
        preferences.getBool(
            (prefix + "en").c_str(),
            false
        );

    preferences.end();


    Serial.print(
        "[WIFI] Load profile "
    );

    Serial.print(
        index
    );

    Serial.print(
        " SSID="
    );

    Serial.print(
        profile.ssid
    );

    Serial.print(
        " Enabled="
    );

    Serial.println(
        profile.enabled
            ? "YES"
            : "NO"
    );


    return
        profile.enabled &&
        profile.ssid.length() > 0U;
}

bool WiFiService::TryConnectProfiles()
{
    for (
        uint8_t index = 0U;
        index < MAX_PROFILES;
        ++index)
    {
        WiFiProfile profile;

        if (!LoadProfile(
                index,
                profile))
        {
            continue;
        }

        Serial.print(
            "[WIFI] Trying profile "
        );

        Serial.print(
            index
        );

        Serial.print(
            ": "
        );

        Serial.println(
            profile.ssid
        );

        WiFi.disconnect();

        WiFi.begin(
            profile.ssid.c_str(),
            profile.password.c_str()
        );

        const uint32_t start =
            millis();

        while (
            millis() - start <
            3000UL)
        {
            if (
                WiFi.status() ==
                WL_CONNECTED)
            {
                Serial.print(
                    "[WIFI] Connected: "
                );

                Serial.println(
                    profile.ssid
                );

                return true;
            }

            delay(
                50
            );
        }
    }

    return false;
}