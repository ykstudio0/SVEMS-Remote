//-------------------------------------------------------------
// File : DataManger.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS
// Version : 0.1.0
// Description : Data 관리
//-------------------------------------------------------------

#include "DataManager.h"


DataManager::SolarData
    DataManager::Solar;

DataManager::BatteryData
    DataManager::Battery;

DataManager::ControllerBatteryData
    DataManager::ControllerBattery;

DataManager::LoadData
    DataManager::Load;

DataManager::TemperatureData
    DataManager::Temperature;

DataManager::SocData
    DataManager::Soc;

DataManager::ChargeData
    DataManager::Charge;

DataManager::EnvironmentData
    DataManager::Environment;

DataManager::CommunicationStats
    DataManager::CommStats;

DataManager::VehicleData 
    DataManager::Vehicle;

void DataManager::ClearUpdates()
{
    Solar.status.updated = false;
    Battery.status.updated = false;
    ControllerBattery.status.updated = false;
    Load.status.updated = false;

    Temperature.cabinStatus.updated = false;
    Temperature.powerBankStatus.updated = false;
    Temperature.controllerStatus.updated = false;

    Soc.status.updated = false;
    Charge.status.updated = false;
    Environment.status.updated = false;
}


void DataManager::UpdateOnlineStatus(
    uint32_t now)
{
    // Remote Display에서는 사용하지 않음.
    //
    // Online / Stale / Offline 상태는
    // MAIN SVEMS에서 판정된 Telemetry 상태를 사용한다.

    (void)now;
}
