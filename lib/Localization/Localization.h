//-------------------------------------------------------------
// File : Localization.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-25
// Project : SVEMS Remote
// Version : 0.1.1
// Description : Localization
//-------------------------------------------------------------
#pragma once

#include <Arduino.h>

namespace Localization
{
    enum class Language : uint8_t
    {
        English = 0U,
        Korean
    };

    struct Text
    {
        const char* english;
        const char* korean;
    };

    // 한글화 대상
    const char* Get(
        const Text& text,
        Language language);

    extern const Text BatterySOC;
    extern const Text BatteryRemaining;
    extern const Text BatteryPower;
    extern const Text BatteryCurrent;
    extern const Text BatteryTemp;
    extern const Text BatteryStatus;
    extern const Text BMSTemp;
    extern const Text ExtTemp;
    extern const Text Capacity;
    extern const Text DtlRemaining;
    extern const Text DtlSOC;
    extern const Text Runtime;
    extern const Text DtlVoltage;
    extern const Text Cell1;
    extern const Text Cell2;
    extern const Text Cell3;
    extern const Text Cell4;
    extern const Text Delta;

    Language GetLanguage();
    void SetLanguage(
        Language language);

    const char* Get(
        const Text& text,
        Language language);

    const char* Get(
        const Text& text);
}