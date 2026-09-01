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

    enum class DataSource : uint8_t
    {
        Main = 0U,
        TestMain
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

    // Header
    extern const Text HeaderOverview;
    extern const Text HeaderSolar;
    extern const Text HeaderBattery;
    extern const Text HeaderLoad;
    extern const Text HeaderTemperature;
    extern const Text HeaderSystem;
    extern const Text HeaderSystemDetail;

    // Contents
    extern const Text OverViewSolar;
    extern const Text OverViewBatRemain;
    extern const Text OverViewSOC;
    // OverView 수정
    // extern const Text OverViewBatteryPower;
    extern const Text OverViewBatteryCurrent;
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
    extern const Text DtlHttpState;
    extern const Text DtlHttpOk;
    extern const Text DtlHttpFail;
    extern const Text DtlHttpConsec;
    extern const Text DtlHttpMax;
    extern const Text DtlHttpError;
    extern const Text DtlSolarOff;
    extern const Text DtlChargeOff;
    extern const Text DtlBMSOff;
    extern const Text DtlLoadOff;
    extern const Text DtlCtrlOff;
    extern const Text DtlSOCOff;
    extern const Text DtlBootCount;
    extern const Text DtlRstReason;
    extern const Text DtlSysWifi;
    extern const Text DtlSysRSSI;
    extern const Text DtlSysHeap;
    extern const Text DtlSysUptime;
    extern const Text DtlSysTelemetry;
    extern const Text DtlSysAge;
    extern const Text DtlSysVehicleBat;
    extern const Text DtlSysReverseChg;

    // Footer
    extern const Text FooterPrev;
    extern const Text FooterNext;

    // Settings UI
    extern const Text SettingsTitle;
    extern const Text SettingsExit;
    extern const Text SettingsWiFi;
    extern const Text SettingsEnglish;
    extern const Text SettingsKorean;

    extern const Text WiFiConfirmTitle;
    extern const Text WiFiConfirmMessage;
    extern const Text WiFiConfirmCancel;
    extern const Text WiFiConfirmOk;

    Language GetLanguage();
    void SetLanguage(
        Language language);

    const char* Get(
        const Text& text,
        Language language);

    const char* Get(
        const Text& text);

    DataSource GetDataSource();

    void SetDataSource(
        DataSource dataSource
    );
}