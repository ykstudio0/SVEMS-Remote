//-------------------------------------------------------------
// File : RemoteTelemetryService.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : SVEMS Telemetry Service
//-------------------------------------------------------------

#include "RemoteTelemetryService.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include "WiFiService.h"
#include "config.h"
#include "Secrets.h"
#include "TelemetryJsonParser.h"
#include "DataManager.h"
#include "RemoteDataBridge.h"
#include "RemoteSystemState.h"
#include "Localization.h"

SVEMS::Telemetry::TelemetryData
    RemoteTelemetryService::m_data;

bool
    RemoteTelemetryService::m_hasData =
        false;

const SVEMS::Telemetry::TelemetryData&
    RemoteTelemetryService::GetData()
{
    return m_data;
}


bool RemoteTelemetryService::HasData()
{
    return m_hasData;
}

namespace
{
    uint32_t lastUpdate =
        0;
}

void RemoteTelemetryService::Update()
{
    const uint32_t now =
        millis();

    if (now - lastUpdate <
            SVEMS::Config::UPDATE_INTERVAL_MS)
    {
        return;
    }

    lastUpdate =
        now;
    
    SVEMS::Remote::RemoteSystemState::
        UpdateFreshness();

    if (!SVEMS::Remote::RemoteSystemState::
            Get().telemetryOnline
    )
    {
        SVEMS::Remote::RemoteDataBridge::
            ApplyOffline();
    }

    if (!WiFiService::IsConnected())
    {
        return;
    }
    
    WiFiClientSecure client;

    client.setInsecure();

    client.setHandshakeTimeout(3);

    //-------------------------------------------------
    // Telemetry Device ID
    //-------------------------------------------------

    const char* deviceId = "unknown";

    if (
        Localization::GetDataSource() ==
        Localization::DataSource::TestMain
    )
    {
        deviceId = "main-test";
    }

    //-------------------------------------------------
    // Telemetry URL
    //-------------------------------------------------

    char telemetryUrl[160];

    snprintf(
        telemetryUrl,
        sizeof(telemetryUrl),
        "%s?deviceId=%s",
        SVEMS::Config::TELEMETRY_URL,
        deviceId
    );

    Serial.print(
        "[REMOTE] URL = "
    );

    Serial.println(
        telemetryUrl
    );
    
    HTTPClient http;

    if (!http.begin(
            client,
            telemetryUrl))
    {
        Serial.println(
            "[REMOTE] HTTP begin failed"
        );

        return;
    }

    http.setConnectTimeout(3000);

    http.setTimeout(3000);

    http.setAuthorization(
        SVEMS::Config::SERVER_USER,
        SVEMS::Config::SERVER_PASSWORD
    );
    
    const int httpCode =
        http.GET();

    Serial.print(
        "[REMOTE] HTTP="
    );

    Serial.println(
        httpCode
    );

    if (httpCode < 0)
    {
        Serial.printf(
            "[REMOTE] HTTP ERROR: %s\n",
            HTTPClient::errorToString(
                httpCode
            ).c_str()
        );

        Serial.printf(
            "[REMOTE] WIFI=%s RSSI=%d IP=%s DNS=%s\n",
            WiFi.isConnected()
                ? "CONNECTED"
                : "DISCONNECTED",
            WiFi.RSSI(),
            WiFi.localIP().toString().c_str(),
            WiFi.dnsIP().toString().c_str()
        );
    }

    if (httpCode == HTTP_CODE_OK)
    {
        const String payload =
            http.getString();

        if (
            SVEMS::Telemetry::
                TelemetryJsonParser::Parse(
                    payload,
                    m_data
                )
        )
        {
            m_hasData =
                true;

            SVEMS::Remote::RemoteDataBridge::Apply(
                m_data
            );

            Serial.println(
                "[REMOTE] Parse OK"
            );

            Serial.printf(
                "[DATA] PV=%.2fV %.2fA %.2fW\n",
                DataManager::Solar.voltage,
                DataManager::Solar.current,
                DataManager::Solar.power
            );

            Serial.printf(
                "[DATA] BAT=%.2fV %.2fA SOC=%u%% TEMP=%.1fC\n",
                DataManager::Battery.voltage,
                DataManager::Battery.current,
                static_cast<unsigned>(
                    DataManager::Soc.value
                ),
                DataManager::Temperature.battery
            );

            Serial.printf(
                "[DATA] ENV=%.1fC %.1f%%\n",
                DataManager::Environment.temperature,
                DataManager::Environment.humidity
            );

            Serial.printf(
                "[REMOTE] RSSI=%lddBm\n",
                static_cast<long>(
                    m_data.system.wifiRssi
                )
            );

            Serial.printf(
                "[DATA] SOLAR ONLINE=%s CHARGE ONLINE=%s\n",
                DataManager::Solar.status.online
                    ? "YES"
                    : "NO",
                DataManager::Charge.status.online
                    ? "YES"
                    : "NO"
            );
        }
        else
        {
            Serial.println(
                "[REMOTE] Parse FAILED"
            );
        }
    }

    http.end();
}