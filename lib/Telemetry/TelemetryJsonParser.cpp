//-------------------------------------------------------------
// File : TelemetryJsonParser.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Telemetry JSON Deserializer
//-------------------------------------------------------------

#include "TelemetryJsonParser.h"

#include <ArduinoJson.h>
#include <string.h>
#include "DataManager.h"
#include "EpeverStatusParser.h"

namespace
{
    SVEMS::Telemetry::TelemetryState ParseTelemetryState(
        const char* value)
    {
        if (value == nullptr)
        {
            return
                SVEMS::Telemetry::TelemetryState::Offline;
        }

        if (strcmp(
                value,
                "Online") == 0)
        {
            return
                SVEMS::Telemetry::TelemetryState::Online;
        }

        if (strcmp(
                value,
                "Stale") == 0)
        {
            return
                SVEMS::Telemetry::TelemetryState::Stale;
        }

        return
            SVEMS::Telemetry::TelemetryState::Offline;
    }


    EpeverStatusParser::ChargingStage ParseChargingStage(
        const char* value)
    {
        if (value == nullptr)
        {
            return
                EpeverStatusParser::ChargingStage::NotCharging;
        }

        if (strcmp(
                value,
                "Float") == 0)
        {
            return
                EpeverStatusParser::ChargingStage::Float;
        }

        if (strcmp(
                value,
                "Boost") == 0)
        {
            return
                EpeverStatusParser::ChargingStage::Boost;
        }

        if (strcmp(
                value,
                "Equalize") == 0)
        {
            return
                EpeverStatusParser::ChargingStage::Equalize;
        }

        return
            EpeverStatusParser::ChargingStage::NotCharging;
    }


    EpeverStatusParser::InputVoltageStatus ParseInputVoltageStatus(
        const char* value)
    {
        if (value == nullptr)
        {
            return
                EpeverStatusParser::InputVoltageStatus::Normal;
        }

        if (strcmp(
                value,
                "NoInputPower") == 0)
        {
            return
                EpeverStatusParser::InputVoltageStatus::NoInputPower;
        }

        if (strcmp(
                value,
                "HighVoltage") == 0)
        {
            return
                EpeverStatusParser::InputVoltageStatus::HighVoltage;
        }

        if (strcmp(
                value,
                "InputError") == 0)
        {
            return
                EpeverStatusParser::InputVoltageStatus::InputError;
        }

        return
            EpeverStatusParser::InputVoltageStatus::Normal;
    }


    uint8_t ParseHttpState(
        const char* value)
    {
        if (value == nullptr)
        {
            return 0U;
        }

        if (strcmp(
                value,
                "Sending") == 0)
        {
            return 1U;
        }

        if (strcmp(
                value,
                "Retry") == 0)
        {
            return 2U;
        }

        return 0U;
    }


    bool ParseTimestamp(
        const char* value,
        SVEMS::Telemetry::TimestampData& timestamp)
    {
        if (value == nullptr)
        {
            return false;
        }

        unsigned year;
        unsigned month;
        unsigned day;
        unsigned hour;
        unsigned minute;
        unsigned second;

        const int result =
            sscanf(
                value,
                "%u-%u-%uT%u:%u:%u",
                &year,
                &month,
                &day,
                &hour,
                &minute,
                &second
            );

        if (result != 6)
        {
            return false;
        }

        timestamp.year =
            static_cast<uint16_t>(
                year
            );

        timestamp.month =
            static_cast<uint8_t>(
                month
            );

        timestamp.day =
            static_cast<uint8_t>(
                day
            );

        timestamp.hour =
            static_cast<uint8_t>(
                hour
            );

        timestamp.minute =
            static_cast<uint8_t>(
                minute
            );

        timestamp.second =
            static_cast<uint8_t>(
                second
            );

        return true;
    }
}


namespace SVEMS::Telemetry
{
    bool TelemetryJsonParser::Parse(
        const String& json,
        TelemetryData& data)
    {
        JsonDocument doc;

        const DeserializationError error =
            deserializeJson(
                doc,
                json
            );

        if (error)
        {
            return false;
        }

        //---------------------------------------------------------
        // Timestamp
        //---------------------------------------------------------

        if (!ParseTimestamp(
                doc["timestamp"],
                data.timestamp))
        {
            return false;
        }

        //---------------------------------------------------------
        // Solar
        //---------------------------------------------------------

        JsonObject solar =
            doc["solar"];

        data.solar.voltage =
            solar["voltage"] | 0.0f;

        data.solar.current =
            solar["current"] | 0.0f;

        data.solar.power =
            solar["power"] | 0.0f;

        data.solar.dailyEnergy =
            solar["dailyEnergy"] | 0.0f;

        data.solar.totalEnergy =
            solar["totalEnergy"] | 0.0f;

        data.solar.stage =
            ParseChargingStage(
                solar["stage"]
            );

        data.solar.inputVoltage =
            ParseInputVoltageStatus(
                solar["input"]
            );

        data.solar.online =
            solar["online"] | false;

        //---------------------------------------------------------
        // Battery
        //---------------------------------------------------------

        JsonObject battery =
            doc["battery"];

        data.battery.voltage =
            battery["voltage"] | 0.0f;

        data.battery.current =
            battery["current"] | 0.0f;

        data.battery.power =
            battery["power"] | 0.0f;

        data.battery.soc =
            battery["soc"] | 0U;

        data.battery.remainingCapacity =
            battery["remainingCapacity"] | 0.0f;

        data.battery.totalCapacity =
            battery["totalCapacity"] | 0.0f;

        data.battery.temperature =
            battery["temperature"] | 0.0f;

        data.battery.cellVoltage[0] =
            battery["cell1"] | 0.0f;

        data.battery.cellVoltage[1] =
            battery["cell2"] | 0.0f;

        data.battery.cellVoltage[2] =
            battery["cell3"] | 0.0f;

        data.battery.cellVoltage[3] =
            battery["cell4"] | 0.0f;

        data.battery.online =
            battery["online"] | false;

        //---------------------------------------------------------
        // Load
        //---------------------------------------------------------

        JsonObject load =
            doc["load"];

        data.load.voltage =
            load["voltage"] | 0.0f;

        data.load.current =
            load["current"] | 0.0f;

        data.load.power =
            load["power"] | 0.0f;

        data.load.online =
            load["online"] | false;


        //---------------------------------------------------------
        // Temperature
        //---------------------------------------------------------

        JsonObject temperature =
            doc["temperature"];

        data.temperature.battery =
            temperature["battery"] | 0.0f;

        data.temperature.bms =
            temperature["bms"] | 0.0f;

        data.temperature.powerBankExternal =
            temperature["powerBankExternal"] | 0.0f;

        data.temperature.controller =
            temperature["controller"] | 0.0f;

        data.temperature.controllerBoard =
            temperature["controllerBoard"] | 0.0f;

        data.temperature.cabinState =
            ParseTelemetryState(
                temperature["cabinState"]
            );

        data.temperature.powerBankState =
            ParseTelemetryState(
                temperature["powerBankState"]
            );

        data.temperature.controllerState =
            ParseTelemetryState(
                temperature["controllerState"]
            );

        //---------------------------------------------------------
        // Environment
        //---------------------------------------------------------

        JsonObject environment =
            doc["environment"];

        data.environment.cabinTemperature =
            environment["temperature"] | 0.0f;

        data.environment.cabinHumidity =
            environment["humidity"] | 0.0f;

        data.environment.online =
            environment["online"] | false;

        //---------------------------------------------------------
        // Communication
        //---------------------------------------------------------

        JsonObject communication =
            doc["communication"];

        data.communication.rs485Ready =
            communication["rs485"] | false;

        data.communication.modbusReady =
            communication["modbus"] | false;

        data.communication.rs485CommunicationError =
            communication[
                "rs485CommunicationError"
            ] | false;

        data.communication.modbusCommunicationError =
            communication[
                "modbusCommunicationError"
            ] | false;

        data.communication.solarState =
            ParseTelemetryState(
                communication["solarState"]
            );

        data.communication.solarTimeoutCount =
            communication["solarTimeout"] | 0U;

        data.communication.solarOfflineCount =
            communication["solarOffline"] | 0U;

        data.communication.chargeState =
            ParseTelemetryState(
                communication["chargeState"]
            );

        data.communication.chargeTimeoutCount =
            communication["chargeTimeout"] | 0U;

        data.communication.chargeOfflineCount =
            communication["chargeOffline"] | 0U;

        data.communication.bmsOfflineCount =
            communication["bmsOffline"] | 0U;

        JsonObject http =
            communication["http"];

        data.communication.http.online =
            http["online"] | false;

        data.communication.http.state =
            ParseHttpState(
                http["state"]
            );

        data.communication.http.successCount =
            http["success"] | 0U;

        data.communication.http.failureCount =
            http["failure"] | 0U;

        data.communication.http.consecutiveFailures =
            http["consecutiveFailures"] | 0U;

        //---------------------------------------------------------
        // HTTP diagnostics
        //---------------------------------------------------------
        data.system.httpState =
            http["state"] | "";

        data.system.httpSuccessCount =
            http["success"] | 0U;

        data.system.httpFailureCount =
            http["failure"] | 0U;

        data.system.httpConsecutiveFailures =
            http["consecutiveFailures"] | 0U;

        data.system.httpMaxConsecutiveFailures =
            http["maxConsecutiveFailures"] | 0U;

        data.system.httpLastErrorCode =
            http["lastErrorCode"] | 0;
        
        //---------------------------------------------------------
        // Vehicle
        //---------------------------------------------------------

        JsonObject vehicle =
            doc["vehicle"];

        data.vehicle.active =
            vehicle["active"] | false;

        data.vehicle.voltage =
            vehicle["voltage"] | 0.0f;

        data.vehicle.reverseChargeEnabled =
            vehicle["reverseChargeEnabled"] | false;

        // Serial.printf(
        //     "[VEHICLE] ACTIVE=%s V=%.3fV REV=%s\n",
        //     data.vehicle.active ? "ON" : "OFF",
        //     data.vehicle.voltage,
        //     data.vehicle.reverseChargeEnabled ? "ON" : "OFF");

        //---------------------------------------------------------
        // System
        //---------------------------------------------------------

        JsonObject system =
            doc["system"];

        data.system.uptimeSeconds =
            system["uptime"] | 0U;

        data.system.wifiConnected =
            system["wifi"] | false;

        data.system.wifiRssi =
            system["rssi"] | 0;

        data.system.deviceCount =
            system["deviceCount"] | 0U;

        data.system.deviceTotal =
            system["deviceTotal"] | 0U;

        data.system.bootCount =
            system["bootCount"] | 0U;

        data.system.resetReason =
            system["resetReason"] | "Unknown";

        JsonObject deviceConfig =
            doc["deviceConfig"];

        data.deviceConfig.mppt =
            deviceConfig["mppt"] | false;

        data.deviceConfig.bms =
            deviceConfig["bms"] | false;

        data.deviceConfig.sht40 =
            deviceConfig["sht40"] | false;

        data.deviceConfig.rtc =
            deviceConfig["rtc"] | false;

        JsonObject deviceStatus =
            doc["deviceStatus"];

        data.deviceStatus.mppt =
            deviceStatus["mppt"] | false;

        data.deviceStatus.bms =
            deviceStatus["bms"] | false;

        data.deviceStatus.sht40 =
            deviceStatus["sht40"] | false;

        data.deviceStatus.rtc =
            deviceStatus["rtc"] | false;

        return true;
    }
}