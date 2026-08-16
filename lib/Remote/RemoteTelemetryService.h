//-------------------------------------------------------------
// File : RemoteTelemetryService.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : SVEMS Telemetry Service
//-------------------------------------------------------------

#pragma once

#include "TelemetryData.h"


class RemoteTelemetryService
{
public:
    static void Update();

    static const SVEMS::Telemetry::TelemetryData&
        GetData();

    static bool HasData();

private:
    static SVEMS::Telemetry::TelemetryData m_data;

    static bool m_hasData;
};