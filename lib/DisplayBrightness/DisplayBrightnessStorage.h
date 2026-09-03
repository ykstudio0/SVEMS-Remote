//-------------------------------------------------------------
// File : DisplayBrightnessStorage.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-03
// Project : SVEMS Remote Display
// Version : 0.1.3
// Description : Display brightness storage management for SVEMS Remote Display
//-------------------------------------------------------------

#pragma once

#include "DisplayBrightnessSettings.h"

namespace SVEMS::Remote::Display
{
    class DisplayBrightnessStorage
    {
    public:
        static bool Load(
            BrightnessSettings& settings);

        static bool Save(
            const BrightnessSettings& settings);
    };
}