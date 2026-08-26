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
        {0, "Solar"},
        {1, "Bat Remain"},
        {2, "SOC"},
        {3, "Load"},
        {4, "Cabin"},
        {5, "Humidity"}
    };
    
    const DisplayLayout::StaticRow g_solarRows[] =
    {
        {0, "PV Voltage"},
        {1, "PV Current"},
        {2, "PV Power"},
        {3, "Stage"},
        {4, "Input"},
        {5, "Daily Eng"}
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
        { 5U, "RUNTIME",    true, DisplayTypes::TextKey::Runtime  }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetailRows[] =
    {
        { 0U, "RSSI",        true },
        { 1U, "EPEVER",      true },
        { 2U, "BMS",         true },
        { 3U, "DEVICES",     true },
        { 4U, "UPTIME",      true },
        { 5U, "HTTP",        true }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetail2Rows[] =
    {
        { 0U, "HTTP STATE",  true },
        { 1U, "HTTP OK",     true },
        { 2U, "HTTP FAIL",   true },
        { 3U, "HTTP CONSEC", true },
        { 4U, "HTTP MAX",    true },
        { 5U, "HTTP ERROR",  true }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetail3Rows[] =
    {
        { 0U, "SOLAR OFF",  true },
        { 1U, "CHARGE OFF", true },
        { 2U, "BMS OFF",    true },
        { 3U, "LOAD OFF",   true },
        { 4U, "CTRL OFF",   true },
        { 5U, "SOC OFF",    true }
    };

    static constexpr DisplayLayout::StaticRow g_systemDetail4Rows[]
    {
        { 0U, "BOOT COUNT",   true },
        { 1U, "RST REASON", true }
    };

    const DisplayLayout::StaticRow g_loadRows[] =
    {
        {0, "Voltage"},
        {1, "Current"},
        {2, "Power"}
    };

    const DisplayLayout::StaticRow g_temperatureRows[] =
    {
        {0, "Cabin"},
        {1, "Battery"},
        {2, "BMS"},
        {3, "Controller"}
    };

    const DisplayLayout::StaticRow g_systemRows[] =
    {
        { 0U, "WIFI",      true },
        { 1U, "RSSI",      true },
        { 2U, "HEAP",      true },
        { 3U, "UPTIME",    true },
        { 4U, "TELEMETRY", true },
        { 5U, "AGE",       true }
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