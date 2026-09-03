//-------------------------------------------------------------
// File : DisplayBrightnessSettings.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-03
// Project : SVEMS Remote Display
// Version : 0.1.3
// Description : Display brightness settings definition for SVEMS Remote Display
//-------------------------------------------------------------

#pragma once

#include <stdint.h>

namespace SVEMS::Remote::Display
{
    enum class BrightnessMode : uint8_t
    {
        Manual = 0,
        Auto   = 1
    };

    struct BrightnessSettings
    {
        BrightnessMode mode =
            BrightnessMode::Manual;

        uint8_t manualPercent = 70U;
    };
}