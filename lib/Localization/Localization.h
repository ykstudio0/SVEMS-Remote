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

    extern const Text OverViewSolar;
    extern const Text OverViewBatRemain;
    extern const Text OverViewSOC;
    extern const Text OverViewLoad;
    extern const Text OverViewCabin;
    extern const Text OverViewHumidity;
    extern const Text PvVoltage;
    extern const Text PvCurrent;
    extern const Text PvPower;
    extern const Text PvStage;
    extern const Text PvInput;
    extern const Text PvDailyEng;
    extern const Text BatterySOC;
    extern const Text BatteryRemaining;
    extern const Text BatteryPower;
    extern const Text BatteryCurrent;
    extern const Text BatteryTemp;
    extern const Text BatteryStatus;
    extern const Text LoadVoltage;
    extern const Text LoadCurrent;
    extern const Text LoadPower;
    extern const Text TemperatureCabin;
    extern const Text TemperatureBattery;
    extern const Text TemperatureBMS;
    extern const Text TemperatureController;
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
    extern const Text DtlRSSI;
    extern const Text DtlEpever;
    extern const Text DtlBMS;
    extern const Text DtlDevices;
    extern const Text DtlUptime;
    extern const Text DtlHttp;

    Language GetLanguage();
    void SetLanguage(
        Language language);

    const char* Get(
        const Text& text,
        Language language);

    const char* Get(
        const Text& text);
}