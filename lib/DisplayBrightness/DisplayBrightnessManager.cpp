//-------------------------------------------------------------
// File : DisplayBrightnessManager.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-03
// Project : SVEMS Remote Display
// Version : 0.1.3
// Description : Display brightness manager definition for SVEMS Remote Display
//-------------------------------------------------------------
#include "DisplayBrightnessManager.h"
#include "DisplayBrightnessStorage.h"

namespace SVEMS::Remote::Display
{
    BrightnessSettings
        DisplayBrightnessManager::Settings;

    void DisplayBrightnessManager::Begin()
    {
        if (!DisplayBrightnessStorage::Load(
                Settings))
        {
            Settings.mode =
                BrightnessMode::Manual;

            Settings.manualPercent =
                70U;

            DisplayBrightnessStorage::Save(
                Settings);
        }
    }

    const BrightnessSettings&
        DisplayBrightnessManager::GetSettings()
    {
        return Settings;
    }

    uint8_t DisplayBrightnessManager::GetBrightness()
    {
        return PercentToBrightness(
            Settings.manualPercent);
    }

    uint8_t
        DisplayBrightnessManager::PercentToBrightness(
            uint8_t percent)
    {
        if (percent > 100U)
        {
            percent = 100U;
        }

        return static_cast<uint8_t>(
            (
                static_cast<uint16_t>(percent)
                * 255U
            ) / 100U);
    }

    bool DisplayBrightnessManager::SetManualPercent(
        uint8_t percent)
    {
        if (percent > 100U)
        {
            percent = 100U;
        }

        Settings.mode =
            BrightnessMode::Manual;

        Settings.manualPercent =
            percent;

        return
            DisplayBrightnessStorage::Save(
                Settings);
    }
}