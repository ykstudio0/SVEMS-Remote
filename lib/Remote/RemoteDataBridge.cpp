//-------------------------------------------------------------
// File : RemoteDataBridge.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : SVEMS RemoteData Bridge
//-------------------------------------------------------------

#include "RemoteDataBridge.h"
#include <Arduino.h>
#include "DataManager.h"
#include "RemoteSystemState.h"


namespace
{
    DataManager::CommunicationState
    ToCommunicationState(
        SVEMS::Telemetry::TelemetryState state)
    {
        switch (state)
        {
            case SVEMS::Telemetry::TelemetryState::Online:
                return
                    DataManager::CommunicationState::Online;

            case SVEMS::Telemetry::TelemetryState::Stale:
                return
                    DataManager::CommunicationState::Stale;

            case SVEMS::Telemetry::TelemetryState::Offline:
            default:
                return
                    DataManager::CommunicationState::Offline;
        }
    }
}


namespace SVEMS::Remote
{
    void RemoteDataBridge::Apply(
        const SVEMS::Telemetry::TelemetryData& data)
    {
        //-----------------------------------------------------
        // Solar
        //-----------------------------------------------------

        DataManager::Solar.voltage =
            data.solar.voltage;

        DataManager::Solar.current =
            data.solar.current;

        DataManager::Solar.power =
            data.solar.power;

        DataManager::Solar.dailyEnergy =
            data.solar.dailyEnergy;

        DataManager::Solar.totalEnergy =
            data.solar.totalEnergy;

        DataManager::Solar.status.online =
            data.solar.online;

        DataManager::Solar.status.state =
            ToCommunicationState(
                data.communication.solarState
            );

        DataManager::Solar.status.updated =
            true;

        DataManager::Solar.status.lastUpdate =
            millis();


        //-----------------------------------------------------
        // Charge
        //-----------------------------------------------------

        DataManager::Charge.stage =
            data.solar.stage;

        DataManager::Charge.inputVoltage =
            data.solar.inputVoltage;

        DataManager::Charge.status.state =
            ToCommunicationState(
                data.communication.chargeState
            );

        DataManager::Charge.status.online =
            (
                data.communication.chargeState !=
                SVEMS::Telemetry::TelemetryState::Offline
            );

        DataManager::Charge.status.updated =
            true;

        DataManager::Charge.status.lastUpdate =
            millis();


        //-----------------------------------------------------
        // Battery
        //-----------------------------------------------------

        DataManager::Battery.voltage =
            data.battery.voltage;

        DataManager::Battery.current =
            data.battery.current;

        DataManager::Battery.power =
            data.battery.power;

        DataManager::Battery.status.online =
            data.battery.online;

        DataManager::Battery.status.updated =
            true;

        DataManager::Battery.status.lastUpdate =
            millis();

        DataManager::Soc.value =
            data.battery.soc;

        DataManager::Battery.remainingCapacity =
            data.battery.remainingCapacity;

        DataManager::Battery.totalCapacity =
            data.battery.totalCapacity;

        DataManager::Soc.status.online =
            data.battery.online;

        DataManager::Soc.status.updated =
            true;

        DataManager::Soc.status.lastUpdate =
            millis();

        DataManager::Temperature.battery =
            data.battery.temperature;

        for (uint8_t i = 0U; i < 4U; ++i)
        {
            DataManager::Battery.cellVoltage[i] =
                data.battery.cellVoltage[i];
        }
        
        DataManager::Temperature.powerBankStatus.online =
            data.battery.online;

        DataManager::Temperature.powerBankStatus.updated =
            true;

        DataManager::Temperature.powerBankStatus.lastUpdate =
            millis();

        //-----------------------------------------------------
        // Load
        //-----------------------------------------------------

        DataManager::Load.voltage =
            data.load.voltage;

        DataManager::Load.current =
            data.load.current;

        DataManager::Load.power =
            data.load.power;

        DataManager::Load.status.online =
            data.load.online;

        DataManager::Load.status.updated =
            true;

        DataManager::Load.status.lastUpdate =
            millis();

        DataManager::Load.status.state =
            data.load.online
                ? DataManager::CommunicationState::Online
                : DataManager::CommunicationState::Offline;

        //-----------------------------------------------------
        // Temperature
        //-----------------------------------------------------

        DataManager::Temperature.battery =
            data.temperature.battery;

        DataManager::Temperature.bms =
            data.temperature.bms;

        DataManager::Temperature.powerBankExternal =
            data.temperature.powerBankExternal;

        DataManager::Temperature.controller =
            data.temperature.controller;

        DataManager::Temperature.controllerBoard =
            data.temperature.controllerBoard;


        DataManager::Temperature.cabinStatus.state =
            ToCommunicationState(
                data.temperature.cabinState
            );

        DataManager::Temperature.cabinStatus.online =
            (
                data.temperature.cabinState !=
                SVEMS::Telemetry::TelemetryState::Offline
            );

        DataManager::Temperature.cabinStatus.updated =
            true;

        DataManager::Temperature.cabinStatus.lastUpdate =
            millis();


        DataManager::Temperature.powerBankStatus.state =
            ToCommunicationState(
                data.temperature.powerBankState
            );

        DataManager::Temperature.powerBankStatus.online =
            (
                data.temperature.powerBankState !=
                SVEMS::Telemetry::TelemetryState::Offline
            );

        DataManager::Temperature.powerBankStatus.updated =
            true;

        DataManager::Temperature.powerBankStatus.lastUpdate =
            millis();


        DataManager::Temperature.controllerStatus.state =
            ToCommunicationState(
                data.temperature.controllerState
            );

        DataManager::Temperature.controllerStatus.online =
            (
                data.temperature.controllerState !=
                SVEMS::Telemetry::TelemetryState::Offline
            );

        DataManager::Temperature.controllerStatus.updated =
            true;

        DataManager::Temperature.controllerStatus.lastUpdate =
            millis();

        //-----------------------------------------------------
        // Environment
        //-----------------------------------------------------

        DataManager::Environment.temperature =
            data.environment.cabinTemperature;

        DataManager::Environment.humidity =
            data.environment.cabinHumidity;

        DataManager::Environment.status.online =
            data.environment.online;

        DataManager::Environment.status.updated =
            true;

        DataManager::Environment.status.lastUpdate =
            millis();


        //-----------------------------------------------------
        // Communication Counters
        //-----------------------------------------------------

        DataManager::CommStats.solarTimeoutCount =
            data.communication.solarTimeoutCount;

        DataManager::CommStats.solarOfflineCount =
            data.communication.solarOfflineCount;

        DataManager::CommStats.chargeTimeoutCount =
            data.communication.chargeTimeoutCount;

        DataManager::CommStats.chargeOfflineCount =
            data.communication.chargeOfflineCount;

        DataManager::CommStats.batteryOfflineCount =
            data.communication.bmsOfflineCount;

        SVEMS::Remote::RemoteSystemState::Update(
            data.timestamp,
            
            data.communication.rs485Ready,
            data.communication.modbusReady,

            data.system.wifiConnected,
            data.system.wifiRssi,

            data.communication.http.online,
            data.communication.http.successCount,
            data.communication.http.failureCount,
            data.communication.http.consecutiveFailures
        );
    }

    void RemoteDataBridge::ApplyOffline()
    {
        //-----------------------------------------------------
        // Solar
        //-----------------------------------------------------

        DataManager::Solar.status.online =
            false;

        DataManager::Solar.status.state =
            DataManager::CommunicationState::Offline;


        //-----------------------------------------------------
        // Charge
        //-----------------------------------------------------

        DataManager::Charge.status.online =
            false;

        DataManager::Charge.status.state =
            DataManager::CommunicationState::Offline;


        //-----------------------------------------------------
        // Battery / SOC
        //-----------------------------------------------------

        DataManager::Battery.status.online =
            false;

        DataManager::Soc.status.online =
            false;


        //-----------------------------------------------------
        // Load
        //-----------------------------------------------------

        DataManager::Load.status.online =
            false;

        DataManager::Load.status.state =
            DataManager::CommunicationState::Offline;


        //-----------------------------------------------------
        // Temperature
        //-----------------------------------------------------

        DataManager::Temperature.cabinStatus.online =
            false;

        DataManager::Temperature.cabinStatus.state =
            DataManager::CommunicationState::Offline;

        DataManager::Temperature.powerBankStatus.online =
            false;

        DataManager::Temperature.powerBankStatus.state =
            DataManager::CommunicationState::Offline;

        DataManager::Temperature.controllerStatus.online =
            false;

        DataManager::Temperature.controllerStatus.state =
            DataManager::CommunicationState::Offline;


        //-----------------------------------------------------
        // Environment
        //-----------------------------------------------------

        DataManager::Environment.status.online =
            false;
    }
}