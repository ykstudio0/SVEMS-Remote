//-------------------------------------------------------------
// File : RemoteTelemetryService.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : SVEMS Telemetry Service
//-------------------------------------------------------------

#pragma once

#include <Arduino.h>

#include "TelemetryData.h"

class RemoteTelemetryService
{
public:
    static bool Begin();

    static void Update();

    static const SVEMS::Telemetry::TelemetryData&
        GetData();

    static bool HasData();

private:
    //---------------------------------------------------------
    // Current Data
    //---------------------------------------------------------

    static SVEMS::Telemetry::TelemetryData
        m_data;

    static bool
        m_hasData;

    //---------------------------------------------------------
    // Worker
    //---------------------------------------------------------

    static TaskHandle_t
        m_workerTask;

    static bool
        m_requestPending;

    static bool
        m_resultReady;

    static SVEMS::Telemetry::TelemetryData
        m_pendingData;

    //---------------------------------------------------------
    // Worker Functions
    //---------------------------------------------------------

    static void WorkerTask(
        void* parameter);

    static void FetchTelemetry();

    static SemaphoreHandle_t
        m_dataMutex;
};