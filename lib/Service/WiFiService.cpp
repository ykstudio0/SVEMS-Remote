#include "WiFiService.h"
#include <WiFi.h>
#include "Config.h"
#include "Secrets.h"

namespace
{
    constexpr uint32_t RETRY_INTERVAL_MS =
        5000UL;

    uint32_t lastRetry =
        0;
}


void WiFiService::Begin()
{
    WiFi.mode(
        WIFI_STA
    );

    WiFi.begin(
        SVEMS::Config::WIFI_SSID,
        SVEMS::Config::WIFI_PASSWORD
    );

    Serial.println(
        "[WIFI] Connecting..."
    );
}

void WiFiService::Update()
{
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

    WiFi.disconnect();

    WiFi.begin(
        SVEMS::Config::WIFI_SSID,
        SVEMS::Config::WIFI_PASSWORD
    );
}

bool WiFiService::IsConnected()
{
    return
        WiFi.status() ==
        WL_CONNECTED;
}