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
    constexpr uint32_t UPDATE_INTERVAL_MS =
        5000UL;

    uint32_t lastUpdate =
        0;
}

void RemoteTelemetryService::Update()
{
    if (!WiFiService::IsConnected())
    {
        return;
    }

    const uint32_t now =
        millis();

    if (now - lastUpdate <
            UPDATE_INTERVAL_MS)
    {
        return;
    }

    lastUpdate =
        now;

    WiFiClientSecure client;

    client.setInsecure();

    HTTPClient http;

    if (!http.begin(
            client,
            SVEMS::Config::TELEMETRY_URL))
    {
        Serial.println(
            "[REMOTE] HTTP begin failed"
        );

        return;
    }

    http.setAuthorization(
        SVEMS::Config::SERVER_USER,
        SVEMS::Config::SERVER_PASSWORD
    );

    // http.addHeader(
    //     "X-SVEMS-API-Key",
    //     SVEMS::Config::SVEMS_API_KEY
    // );
    
    const int httpCode =
        http.GET();

    Serial.print(
        "[REMOTE] HTTP="
    );

    Serial.println(
        httpCode
    );

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