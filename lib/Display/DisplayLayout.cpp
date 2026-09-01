//-------------------------------------------------------------
// File : DisplayLayout.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-07-29
// Project : SVEMS
// Version : 0.4.7
// Description : Display Layout
//-------------------------------------------------------------

#include "DisplayLayout.h"

namespace
{
    using namespace DisplayLayout;

    // 한글화 대상
    const DisplayLayout::StaticRow g_overviewRows[] =
    {
        {0, "Solar",        true, DisplayTypes::TextKey::OverViewSolar },
        {1, "Bat Remain",   true, DisplayTypes::TextKey::OverViewBatRemain },
        {2, "SOC",          true, DisplayTypes::TextKey::OverViewSOC },
        {3, "Bat Power",    true, DisplayTypes::TextKey::OverViewBatteryPower },
        {4, "Cabin",        true, DisplayTypes::TextKey::OverViewCabin },
        {5, "Humidity",     true, DisplayTypes::TextKey::OverViewHumidity }
    };
    
    const DisplayLayout::StaticRow g_solarRows[] =
    {
        {0, "PV Voltage",   true, DisplayTypes::TextKey::PvVoltage },
        {1, "PV Current",   true, DisplayTypes::TextKey::PvCurrent },
        {2, "PV Power",     true, DisplayTypes::TextKey::PvPower },
        {3, "Stage",        true, DisplayTypes::TextKey::PvStage },
        {4, "Input",        true, DisplayTypes::TextKey::PvInput },
        {5, "Daily Eng",    true, DisplayTypes::TextKey::PvDailyEng }
    };

    const DisplayLayout::StaticRow g_batteryRows[] =
    {
        {0, "SOC",          true, DisplayTypes::TextKey::BatterySOC },
        {1, "Remaining",    true, DisplayTypes::TextKey::BatteryRemaining },
        {2, "Power",        true, DisplayTypes::TextKey::BatteryPower },
        {3, "Current",      true, DisplayTypes::TextKey::BatteryCurrent },
        {4, "Temp",         true, DisplayTypes::TextKey::BatteryTemp },
        {5, "Status",       true, DisplayTypes::TextKey::BatteryStatus }
    };

    constexpr StaticRow g_batteryDetailRows[] =
    {
        { 0U, "Voltage",   true, DisplayTypes::TextKey::DtlVoltage },
        { 1U, "Cell 1",    true, DisplayTypes::TextKey::Cell1 },
        { 2U, "Cell 2",    true, DisplayTypes::TextKey::Cell2 },
        { 3U, "Cell 3",    true, DisplayTypes::TextKey::Cell3 },
        { 4U, "Cell 4",    true, DisplayTypes::TextKey::Cell4 },
        { 5U, "Delta",     true, DisplayTypes::TextKey::Delta }
    };

    constexpr StaticRow g_batteryDetail2Rows[] =
    {
        { 0U, "BMS Temp",   true, DisplayTypes::TextKey::BMSTemp },
        { 1U, "Ext Temp",   true, DisplayTypes::TextKey::ExtTemp },
        { 2U, "Capacity",   true, DisplayTypes::TextKey::Capacity },
        { 3U, "Remaining",  true, DisplayTypes::TextKey::DtlRemaining },
        { 4U, "SOC",        true, DisplayTypes::TextKey::DtlSOC },
        { 5U, "RUNTIME",    true, DisplayTypes::TextKey::Runtime }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetailRows[] =
    {
        { 0U, "RSSI",        true, DisplayTypes::TextKey::DtlRSSI },
        { 1U, "EPEVER",      true, DisplayTypes::TextKey::DtlEpever },
        { 2U, "BMS",         true, DisplayTypes::TextKey::DtlBMS },
        { 3U, "DEVICES",     true, DisplayTypes::TextKey::DtlDevices },
        { 4U, "UPTIME",      true, DisplayTypes::TextKey::DtlUptime },
        { 5U, "HTTP",        true, DisplayTypes::TextKey::DtlHttp }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetail2Rows[] =
    {
        { 0U, "HTTP STATE",  true, DisplayTypes::TextKey::DtlHttpState },
        { 1U, "HTTP OK",     true, DisplayTypes::TextKey::DtlHttpOk },
        { 2U, "HTTP FAIL",   true, DisplayTypes::TextKey::DtlHttpFail },
        { 3U, "HTTP CONSEC", true, DisplayTypes::TextKey::DtlHttpConsec },
        { 4U, "HTTP MAX",    true, DisplayTypes::TextKey::DtlHttpMax },
        { 5U, "HTTP ERROR",  true, DisplayTypes::TextKey::DtlHttpError }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetail3Rows[] =
    {
        { 0U, "SOLAR OFF",  true, DisplayTypes::TextKey::DtlSolarOff },
        { 1U, "CHARGE OFF", true, DisplayTypes::TextKey::DtlChargeOff },
        { 2U, "BMS OFF",    true, DisplayTypes::TextKey::DtlBMSOff },
        { 3U, "LOAD OFF",   true, DisplayTypes::TextKey::DtlLoadOff },
        { 4U, "CTRL OFF",   true, DisplayTypes::TextKey::DtlCtrlOff },
        { 5U, "SOC OFF",    true, DisplayTypes::TextKey::DtlSOCOff }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetail4Rows[]
    {
        { 0U, "BOOT COUNT",     true, DisplayTypes::TextKey::DtlBootCount },
        { 1U, "RST REASON",     true, DisplayTypes::TextKey::DtlRstReason }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetail5Rows[]
    {
        { 0U, "Vehicle Bat",     true, DisplayTypes::TextKey::DtlSysVehicleBat },
        { 1U, "Reverse Chg",     true, DisplayTypes::TextKey::DtlSysReverseChg }
    };

    const DisplayLayout::StaticRow g_loadRows[] =
    {
        {0, "Voltage",      true, DisplayTypes::TextKey::LoadVoltage },
        {1, "Current",      true, DisplayTypes::TextKey::LoadCurrent },
        {2, "Power",        true, DisplayTypes::TextKey::LoadPower }
    };

    const DisplayLayout::StaticRow g_temperatureRows[] =
    {
        {0, "Cabin",        true, DisplayTypes::TextKey::TemperatureCabin },
        {1, "Battery",      true, DisplayTypes::TextKey::TemperatureBattery },
        {2, "BMS",          true, DisplayTypes::TextKey::TemperatureBMS },
        {3, "Controller",   true, DisplayTypes::TextKey::TemperatureController }
    };

    const DisplayLayout::StaticRow g_systemRows[] =
    {
        { 0U, "WIFI",      true, DisplayTypes::TextKey::DtlSysWifi },
        { 1U, "RSSI",      true, DisplayTypes::TextKey::DtlSysRSSI },
        { 2U, "HEAP",      true, DisplayTypes::TextKey::DtlSysHeap },
        { 3U, "UPTIME",    true, DisplayTypes::TextKey::DtlSysUptime },
        { 4U, "TELEMETRY", true, DisplayTypes::TextKey::DtlSysTelemetry },
        { 5U, "AGE",       true, DisplayTypes::TextKey::DtlSysAge }
    };

} // anonymous namespace

const DisplayLayout::StaticRow*
    DisplayLayout::GetStaticRows(
        DisplayPages::Page page,
        uint8_t subPage,
        size_t& count)
    {
        switch(page)
        {
            case DisplayPages::Page::Overview:
                count =
                    sizeof(g_overviewRows) /
                    sizeof(g_overviewRows[0]);

                return g_overviewRows;

            case DisplayPages::Page::Solar:
                count =
                    sizeof(g_solarRows) /
                    sizeof(g_solarRows[0]);

                return g_solarRows;

            case DisplayPages::Page::Battery:
            {
                if (subPage == 1U)
                {
                    count =
                        sizeof(g_batteryDetailRows) /
                        sizeof(g_batteryDetailRows[0]);

                    return g_batteryDetailRows;
                }

                if (subPage == 2U)
                {
                    count =
                        sizeof(g_batteryDetail2Rows) /
                        sizeof(g_batteryDetail2Rows[0]);

                    return g_batteryDetail2Rows;
                }

                count =
                    sizeof(g_batteryRows) /
                    sizeof(g_batteryRows[0]);

                return g_batteryRows;
            }

            case DisplayPages::Page::Load:
                count =
                    sizeof(g_loadRows) /
                    sizeof(g_loadRows[0]);

                return g_loadRows;

            case DisplayPages::Page::Temperature:
                count =
                    sizeof(g_temperatureRows) /
                    sizeof(g_temperatureRows[0]);

                return g_temperatureRows;

            case DisplayPages::Page::System:
            {
                if (subPage == 1U)
                {
                    count =
                        sizeof(g_systemDetailRows) /
                        sizeof(g_systemDetailRows[0]);

                    return g_systemDetailRows;
                }

                if (subPage == 2U)
                {
                    count =
                        sizeof(g_systemDetail2Rows) /
                        sizeof(g_systemDetail2Rows[0]);

                    return g_systemDetail2Rows;
                }

                if (subPage == 3U)
                {
                    count =
                        sizeof(g_systemDetail3Rows) /
                        sizeof(g_systemDetail3Rows[0]);

                    return g_systemDetail3Rows;
                }

                if (subPage == 4U)
                {
                    count =
                        sizeof(g_systemDetail4Rows) /
                        sizeof(g_systemDetail4Rows[0]);

                    return g_systemDetail4Rows;
                }

                if (subPage == 5U)
                {
                    count =
                        sizeof(g_systemDetail5Rows) /
                        sizeof(g_systemDetail5Rows[0]);

                    return g_systemDetail5Rows;
                }

                count =
                    sizeof(g_systemRows) /
                    sizeof(g_systemRows[0]);

                return g_systemRows;
            }

            default:
                count = 0;
                return nullptr;
        }
    }