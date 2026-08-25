//-------------------------------------------------------------
// File : Localization.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-25
// Project : SVEMS Remote
// Version : 0.1.1
// Description : Localization
//-------------------------------------------------------------

#include "Localization.h"

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

    const Text Battery =
    {
        "BATTERY",
        "배터리"
    };

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

    const Text Remaining =
    {
        "Remaining",
        "잔여용량"
    };

    const Text SOC =
    {
        "SOC",
        "충전잔량"
    };

    const Text Runtime =
    {
        "RUNTIME",
        "가용시간"
    };
}

