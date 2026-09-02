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

            bool rs485CommunicationError = false;
            bool modbusCommunicationError = false;

            bool mainWifiConnected = false;
            int32_t mainWifiRssi = 0;

            bool mainHttpOnline = false;
            uint32_t mainUptimeSeconds = 0U;

            uint32_t mainBootCount =
                0U;

            String mainResetReason =
                "Unknown";

            String httpState;

            uint32_t httpSuccessCount = 0U;
            uint32_t httpFailureCount = 0U;
            uint32_t httpConsecutiveFailures = 0U;
            uint32_t httpMaxConsecutiveFailures = 0U;

            int32_t httpLastErrorCode = 0;

            uint32_t deviceCount = 0U;
            uint32_t deviceTotal = 0U;

            bool mpptConfigured = false;
            bool bmsConfigured = false;
            bool sht40Configured = false;
            bool rtcConfigured = false;

            bool mpptOnline = false;
            bool bmsOnline = false;
            bool sht40Online = false;
            bool rtcOnline = false;
        };

        static const State& Get();

        static void Update(
            const SVEMS::Telemetry::TimestampData& timestamp,
            bool rs485Ready,
            bool modbusReady,
            bool rs485CommunicationError,
            bool modbusCommunicationError,
            bool mainWifiConnected,
            int32_t mainWifiRssi,
            uint32_t mainUptimeSeconds,
            uint32_t mainBootCount,
            const String& mainResetReason,
            uint32_t deviceCount,
            uint32_t deviceTotal,
            bool mpptConfigured,
            bool bmsConfigured,
            bool sht40Configured,
            bool rtcConfigured,
            bool mpptOnline,
            bool bmsOnline,
            bool sht40Online,
            bool rtcOnline,
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