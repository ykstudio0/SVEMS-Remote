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

    // 한글과 대상
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
}

