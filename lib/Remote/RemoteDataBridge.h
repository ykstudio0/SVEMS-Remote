//-------------------------------------------------------------
// File : RemoteDataBridge.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : SVEMS RemoteData Bridge
//-------------------------------------------------------------

#pragma once

#include "TelemetryData.h"


namespace SVEMS::Remote
{
    class RemoteDataBridge
    {
    public:
        static void Apply(
            const SVEMS::Telemetry::TelemetryData& data
        );
    };
}