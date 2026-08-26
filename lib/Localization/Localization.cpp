//-------------------------------------------------------------
// File : Localization.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-25
// Project : SVEMS Remote
// Version : 0.1.1
// Description : Localization
//-------------------------------------------------------------

#include "Localization.h"
#include <Preferences.h>

namespace
{
    constexpr const char* NVS_NAMESPACE = "display";
    constexpr const char* NVS_KEY_LANGUAGE = "language";
}

namespace Localization
{
    static Language g_language =
        Language::English;

    Language GetLanguage()
    {
        return g_language;
    }

    void SetLanguage(
        Language language)
    {
        g_language =
            language;
    }
    
    const char* Get(
        const Text& text,
        Language language)
    {
        if (language == Language::Korean)
        {
            return text.korean;
        }

        return text.english;
    }

    const char* Get(
        const Text& text)
    {
        return Get(
            text,
            g_language);
    }

    // 한글화 대상
    // OverView Main
    const Text OverViewSolar =
    {
        "Solar",
        "태양광"
    };
    
    const Text OverViewBatRemain =
    {
        "Bat Remain",
        "배터리량"
    };

    const Text OverViewSOC =
    {
        "SOC",
        "충전잔량"
    };

    const Text OverViewLoad =
    {
        "Load",
        "부하"
    };

    const Text OverViewCabin =
    {
        "Cabin",
        "실내온도"
    };

    const Text OverViewHumidity =
    {
        "Humidity",
        "실내습도"
    };

    //Solar Main
    const Text PvVoltage =
    {
        "Pv Voltage",
        "Pv전압"
    };

    const Text PvCurrent =
    {
        "Pv Current",
        "Pv전류"
    };

    const Text PvPower =
    {
        "Pv Power",
        "Pv전력"
    };

    const Text PvStage =
    {
        "Stage",
        "충전단계"
    };

    const Text PvInput =
    {
        "Input",
        "입력상태"
    };

    const Text PvDailyEng =
    {
        "Daily Eng",
        "일발전량"
    };

    // Battery Main
    const Text BatterySOC =
    {
        "SOC",
        "충전잔량"
    };

    const Text BatteryRemaining =
    {
        "Remaining",
        "잔여용량"
    };

    const Text BatteryPower =
    {
        "Power",
        "전력"
    };

    const Text BatteryCurrent =
    {
        "Current",
        "전류"
    };

    const Text BatteryTemp =
    {
        "Temp",
        "온도"
    };

    const Text BatteryStatus =
    {
        "Status",
        "상태"
    };

    // Load Main
    const Text LoadVoltage =
    {
        "Voltage",
        "전압"
    };

    const Text LoadCurrent =
    {
        "Current",
        "전류"
    };

    const Text LoadPower =
    {
        "Power",
        "전력"
    };

    // Temperature Main
    const Text TemperatureCabin =
    {
        "Cabin",
        "실내"
    };

    const Text TemperatureBattery =
    {
        "Battery",
        "배터리"
    };

    const Text TemperatureBMS =
    {
        "BMS",
        "BMS"
    };

    const Text TemperatureController =
    {
        "Controller",
        "컨트롤러"
    };

    // Battery Dtl(2)
    const Text BMSTemp =
    {
        "BMS Temp",
        "BMS온도"
    };

    const Text ExtTemp =
    {
        "Ext Temp",
        "외부온도"
    };

    const Text Capacity =
    {
        "Capacity",
        "용량"
    };

    const Text DtlRemaining =
    {
        "Remaining",
        "잔여용량"
    };

    const Text DtlSOC =
    {
        "SOC",
        "충전잔량"
    };

    const Text Runtime =
    {
        "RUNTIME",
        "가용시간"
    };

    // Battery Dtl(1)
    const Text DtlVoltage =
    {
        "Voltage",
        "전압"
    };

    const Text Cell1 =
    {
        "Cell1",
        "셀 1"
    };

    const Text Cell2 =
    {
        "Cell2",
        "셀 2"
    };

    const Text Cell3 =
    {
        "Cell3",
        "셀 3"
    };

    const Text Cell4 =
    {
        "Cell4",
        "셀 4"
    };

    const Text Delta =
    {
        "Delta",
        "셀편차"
    };

    const Text DtlRSSI =
    {
        "RSSI",
        "RSSI"
    };

    const Text DtlEpever =
    {
        "EPEVER",
        "EPEVER"
    };

    const Text DtlBMS =
    {
        "BMS",
        "BMS"
    };

    const Text DtlDevices =
    {
        "Devices",
        "장치"
    };

    const Text DtlUptime =
    {
        "Uptime",
        "가동시간"
    };

    const Text DtlHttp =
    {
        "HTTP",
        "HTTP"
    };
}

