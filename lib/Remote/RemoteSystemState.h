//-------------------------------------------------------------
// File : RemoteSystemState.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Stores MAIN SVEMS system state received remotely
//-------------------------------------------------------------

#pragma once

#include <stdint.h>
#include "TelemetryData.h"

namespace SVEMS::Remote
{
    class RemoteSystemState
    {
    public:
        struct State
        {
            SVEMS::Telemetry::TimestampData timestamp;

            uint32_t timestampReceivedMillis = 0U;

            bool telemetryReceived = false;
            bool telemetryOnline = false;

            bool rs485Ready = false;
            bool modbusReady = false;

            bool mainWifiConnected = false;
            int32_t mainWifiRssi = 0;

            bool mainHttpOnline = false;
            uint32_t mainUptimeSeconds = 0U;

            String httpState;

            uint32_t httpSuccessCount = 0U;
            uint32_t httpFailureCount = 0U;
            uint32_t httpConsecutiveFailures = 0U;
            uint32_t httpMaxConsecutiveFailures = 0U;

            int32_t httpLastErrorCode = 0;
        };

        static const State& Get();

        static void Update(
            const SVEMS::Telemetry::TimestampData& timestamp,
            bool rs485Ready,
            bool modbusReady,
            bool mainWifiConnected,
            int32_t mainWifiRssi,
            uint32_t mainUptimeSeconds,
            bool mainHttpOnline,
            const String& httpState,
            uint32_t httpSuccessCount,
            uint32_t httpFailureCount,
            uint32_t httpConsecutiveFailures,
            uint32_t httpMaxConsecutiveFailures,
            int32_t httpLastErrorCode
        );

        static void UpdateFreshness();

    private:
        static bool IsSameTimestamp(
            const SVEMS::Telemetry::TimestampData& a,
            const SVEMS::Telemetry::TimestampData& b
        );    
    
        static State m_state;
    };
}