//-------------------------------------------------------------
// File : DisplayBrightnessManager.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-03
// Project : SVEMS Remote Display
// Version : 0.1.3
// Description : Display brightness manager definition for SVEMS Remote Display
//-------------------------------------------------------------
#pragma once

#include <stdint.h>

#include "DisplayBrightnessSettings.h"

namespace SVEMS::Remote::Display
{
    class DisplayBrightnessManager
    {
    public:
        static void Begin();

        static const BrightnessSettings&
            GetSettings();

        static uint8_t GetBrightness();

        static uint8_t PercentToBrightness(
            uint8_t percent);

        static bool SetManualPercent(
            uint8_t percent);

        static bool SaveSettings(
            BrightnessMode mode,
            uint8_t manualPercent);

        static void SetAutoBrightnessPercent(
            uint8_t percent);

        static uint8_t GetAutoBrightnessPercent();

    private:
        static BrightnessSettings Settings;

        static uint8_t AutoBrightnessPercent;
    };
}