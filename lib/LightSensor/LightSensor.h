//-------------------------------------------------------------
// File : LightSensor.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-04
// Project : SVEMS Remote
// Version : 0.1.3
// Description : Light Sensor Management
//-------------------------------------------------------------
#pragma once

#include <stdint.h>

namespace SVEMS::Remote
{
    class LightSensor
    {
    public:
        static void Begin();

        static void Update();

        static uint16_t GetRaw();
        static uint16_t GetFiltered();

        static uint8_t GetBrightnessPercent();

        static uint8_t GetStableBrightnessPercent();

    private:
        static uint16_t RawValue;
        static uint16_t FilteredValue;

        static bool Initialized;

        static uint8_t CandidateBrightness;
        static uint8_t StableBrightness;
        static uint8_t CandidateCount;

        static constexpr uint8_t
            BRIGHTNESS_STABLE_COUNT = 3U;
    };
}