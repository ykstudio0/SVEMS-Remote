//-------------------------------------------------------------
// File : RemoteSystemState.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Stores MAIN SVEMS system state received remotely
//-------------------------------------------------------------

#include "RemoteSystemState.h"


namespace SVEMS::Remote
{
    RemoteSystemState::State
        RemoteSystemState::m_state;


    const RemoteSystemState::State&
    RemoteSystemState::Get()
    {
        return m_state;
    }


    void RemoteSystemState::Update(
        const SVEMS::Telemetry::TimestampData& timestamp,
        bool rs485Ready,
        bool modbusReady,
        bool mainWifiConnected,
        int32_t mainWifiRssi,
        bool mainHttpOnline,
        uint32_t httpSuccessCount,
        uint32_t httpFailureCount,
        uint32_t httpConsecutiveFailures)
    {
        m_state.timestamp =
            timestamp;

        m_state.timestampReceivedMillis =
            millis();

        m_state.rs485Ready =
            rs485Ready;

        m_state.modbusReady =
            modbusReady;

        m_state.mainWifiConnected =
            mainWifiConnected;

        m_state.mainWifiRssi =
            mainWifiRssi;

        m_state.mainHttpOnline =
            mainHttpOnline;

        m_state.httpSuccessCount =
            httpSuccessCount;

        m_state.httpFailureCount =
            httpFailureCount;

        m_state.httpConsecutiveFailures =
            httpConsecutiveFailures;
    }
}