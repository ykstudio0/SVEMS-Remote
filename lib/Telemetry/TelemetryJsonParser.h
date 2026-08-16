//-------------------------------------------------------------
// File : TelemetryParser.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : SVEMS Telemetry Data Model
//-------------------------------------------------------------

#pragma once

#include <Arduino.h>

#include "TelemetryData.h"


namespace SVEMS::Telemetry
{
    class TelemetryJsonParser
    {
    public:
        static bool Parse(
            const String& json,
            TelemetryData& data
        );
    };
}