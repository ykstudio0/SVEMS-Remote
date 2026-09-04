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

TaskHandle_t
    RemoteTelemetryService::m_workerTask =
        nullptr;

bool
    RemoteTelemetryService::m_requestPending =
        false;

bool
    RemoteTelemetryService::m_resultReady =
        false;

SVEMS::Telemetry::TelemetryData
    RemoteTelemetryService::m_pendingData;

SemaphoreHandle_t
    RemoteTelemetryService::m_dataMutex =
        nullptr;

bool RemoteTelemetryService::Begin()
{
    if (m_workerTask != nullptr)
    {
        return true;
    }

    //-------------------------------------------------
    // Data Mutex
    //-------------------------------------------------

    m_dataMutex =
        xSemaphoreCreateMutex();

    if (m_dataMutex == nullptr)
    {
        Serial.println(
            "[REMOTE] Telemetry Mutex Failed"
        );

        return false;
    }

    //-------------------------------------------------
    // Worker
    //-------------------------------------------------

    const BaseType_t result =
        xTaskCreatePinnedToCore(
            WorkerTask,
            "TelemetryWorker",
            10240,
            nullptr,
            1,
            &m_workerTask,
            0
        );

    if (result != pdPASS)
    {
        m_workerTask =
            nullptr;

        vSemaphoreDelete(
            m_dataMutex);

        m_dataMutex =
            nullptr;

        Serial.println(
            "[REMOTE] Telemetry Worker Failed"
        );

        return false;
    }

    Serial.println(
        "[REMOTE] Telemetry Worker Ready"
    );

    return true;
}

void RemoteTelemetryService::WorkerTask(
    void* parameter)
{
    (void)parameter;

    while (true)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        FetchTelemetry();

        m_requestPending =
            false;
    }
}

void RemoteTelemetryService::FetchTelemetry()
{
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

        http.end();
        return;
    }

    if (httpCode == HTTP_CODE_OK)
    {
        const String payload =
            http.getString();

        SVEMS::Telemetry::TelemetryData
            receivedData;

        if (
            SVEMS::Telemetry::
                TelemetryJsonParser::Parse(
                    payload,
                    receivedData
                )
        )
        {
            if (
                xSemaphoreTake(
                    m_dataMutex,
                    pdMS_TO_TICKS(100)
                ) == pdTRUE
            )
            {
                m_pendingData =
                    receivedData;

                m_resultReady =
                    true;

                xSemaphoreGive(
                    m_dataMutex
                );
            }

            Serial.println(
                "[REMOTE] Parse OK"
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

    //-------------------------------------------------
    // Freshness
    //-------------------------------------------------

    SVEMS::Remote::RemoteSystemState::
        UpdateFreshness();

    if (!SVEMS::Remote::RemoteSystemState::
            Get().telemetryOnline)
    {
        SVEMS::Remote::RemoteDataBridge::
            ApplyOffline();
    }

    //-------------------------------------------------
    // Worker Result
    //-------------------------------------------------

    if (m_resultReady)
    {
        SVEMS::Telemetry::TelemetryData
            receivedData;

        bool hasResult =
            false;

        if (
            xSemaphoreTake(
                m_dataMutex,
                pdMS_TO_TICKS(10)
            ) == pdTRUE
        )
        {
            if (m_resultReady)
            {
                receivedData =
                    m_pendingData;

                m_resultReady =
                    false;

                hasResult =
                    true;
            }

            xSemaphoreGive(
                m_dataMutex
            );
        }

        if (hasResult)
        {
            m_data =
                receivedData;

            m_hasData =
                true;

            SVEMS::Remote::RemoteDataBridge::
                Apply(
                    m_data
                );
        }
    }

    //-------------------------------------------------
    // Poll Interval
    //-------------------------------------------------

    if (
        now - lastUpdate <
        SVEMS::Config::UPDATE_INTERVAL_MS
    )
    {
        return;
    }

    lastUpdate =
        now;

    //-------------------------------------------------
    // Worker Busy
    //-------------------------------------------------

    if (m_requestPending)
    {
        return;
    }

    if (!WiFiService::IsConnected())
    {
        return;
    }

    //-------------------------------------------------
    // Request Worker
    //-------------------------------------------------

    m_requestPending =
        true;

    xTaskNotifyGive(
        m_workerTask
    );
}