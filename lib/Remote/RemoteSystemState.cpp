//-------------------------------------------------------------
// File : RemoteSystemState.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Stores MAIN SVEMS system state received remotely
//-------------------------------------------------------------

#include "RemoteSystemState.h"
#include "Config.h"
#include <Arduino.h>

namespace SVEMS::Remote
{
    RemoteSystemState::State
        RemoteSystemState::m_state;


    const RemoteSystemState::State&
    RemoteSystemState::Get()
    {
        return m_state;
    }

    bool RemoteSystemState::IsSameTimestamp(
        const SVEMS::Telemetry::TimestampData& a,
        const SVEMS::Telemetry::TimestampData& b)
    {
        return
            a.year   == b.year   &&
            a.month  == b.month  &&
            a.day    == b.day    &&
            a.hour   == b.hour   &&
            a.minute == b.minute &&
            a.second == b.second;
    }

    void RemoteSystemState::Update(
        const SVEMS::Telemetry::TimestampData& timestamp,
        bool rs485Ready,
        bool modbusReady,
        bool mainWifiConnected,
        int32_t mainWifiRssi,
        uint32_t mainUptimeSeconds,
        uint32_t deviceCount,
        uint32_t deviceTotal,
        bool mainHttpOnline,
        const String& httpState,
        uint32_t httpSuccessCount,
        uint32_t httpFailureCount,
        uint32_t httpConsecutiveFailures,
        uint32_t httpMaxConsecutiveFailures,
        int32_t httpLastErrorCode)
    {
        //-----------------------------------------------------
        // Telemetry Timestamp
        //-----------------------------------------------------

        const bool newTelemetry =
            !m_state.telemetryReceived ||
            !IsSameTimestamp(
                m_state.timestamp,
                timestamp
            );

        if (newTelemetry)
        {
            m_state.timestamp =
                timestamp;

            m_state.timestampReceivedMillis =
                millis();

            m_state.telemetryReceived =
                true;

            m_state.telemetryOnline =
                true;
        }

        //-----------------------------------------------------
        // MAIN System State
        //-----------------------------------------------------

        m_state.rs485Ready =
            rs485Ready;

        m_state.modbusReady =
            modbusReady;

        m_state.mainWifiConnected =
            mainWifiConnected;

        m_state.mainWifiRssi =
            mainWifiRssi;

        m_state.mainUptimeSeconds =
            mainUptimeSeconds;

        m_state.mainHttpOnline =
            mainHttpOnline;

        m_state.httpState =
            httpState;

        m_state.httpSuccessCount =
            httpSuccessCount;

        m_state.httpFailureCount =
            httpFailureCount;

        m_state.httpConsecutiveFailures =
            httpConsecutiveFailures;

        m_state.httpMaxConsecutiveFailures =
            httpMaxConsecutiveFailures;

        m_state.httpLastErrorCode =
            httpLastErrorCode;
    }


    void RemoteSystemState::UpdateFreshness()
    {
        if (!m_state.telemetryReceived)
        {
            m_state.telemetryOnline =
                false;

            return;
        }

        if (
            millis() -
            m_state.timestampReceivedMillis >=
                SVEMS::Config::TELEMETRY_TIMEOUT_MS
        )
        {
            m_state.telemetryOnline =
                false;
        }
    }
}