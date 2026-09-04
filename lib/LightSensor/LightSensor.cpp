//-------------------------------------------------------------
// File : LightSensor.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-04
// Project : SVEMS Remote
// Version : 0.1.3
// Description : Light Sensor Management
//-------------------------------------------------------------
#include <Arduino.h>
#include "LightSensor.h"
#include "Pins.h"

namespace SVEMS::Remote
{
    uint16_t LightSensor::RawValue = 0U;
    uint16_t LightSensor::FilteredValue = 0U;

    uint8_t LightSensor::CandidateBrightness = 0U;
    uint8_t LightSensor::StableBrightness = 0U;
    uint8_t LightSensor::CandidateCount = 0U;

    bool LightSensor::Initialized = false;

    void LightSensor::Begin()
    {
        pinMode(
            PIN_LIGHT_SENSOR,
            INPUT);

        RawValue =
            analogRead(
                PIN_LIGHT_SENSOR);

        FilteredValue =
            RawValue;

        Initialized =
            true;

        const uint8_t initialBrightness =
            GetBrightnessPercent();

        CandidateBrightness =
            initialBrightness;

        StableBrightness =
            initialBrightness;

        CandidateCount =
            BRIGHTNESS_STABLE_COUNT;
    }

    void LightSensor::Update()
    {
        if (!Initialized)
        {
            return;
        }

        RawValue =
            analogRead(
                PIN_LIGHT_SENSOR);

        //-------------------------------------------------
        // Simple IIR Filter
        //
        // filtered =
        //     filtered * 7/8 +
        //     raw      * 1/8
        //-------------------------------------------------

        FilteredValue =
            static_cast<uint16_t>(
                (
                    static_cast<uint32_t>(
                        FilteredValue) * 7U
                    +
                    static_cast<uint32_t>(
                        RawValue)
                ) / 8U);

        const uint8_t candidate =
            GetBrightnessPercent();

        if (candidate == CandidateBrightness)
        {
            if (
                CandidateCount <
                BRIGHTNESS_STABLE_COUNT
            )
            {
                ++CandidateCount;
            }
        }
        else
        {
            CandidateBrightness =
                candidate;

            CandidateCount =
                1U;
        }

        if (
            CandidateCount >=
            BRIGHTNESS_STABLE_COUNT
        )
        {
            StableBrightness =
                CandidateBrightness;
        }
    }

    uint16_t LightSensor::GetRaw()
    {
        return RawValue;
    }

    uint16_t LightSensor::GetFiltered()
    {
        return FilteredValue;
    }

    uint8_t LightSensor::GetBrightnessPercent()
    {
        const uint16_t value =
            FilteredValue;

        if (value <= 200U)
        {
            return 5U;
        }

        if (value <= 500U)
        {
            return 10U;
        }

        if (value <= 1000U)
        {
            return 20U;
        }

        if (value <= 1800U)
        {
            return 35U;
        }

        if (value <= 2600U)
        {
            return 50U;
        }

        if (value <= 3200U)
        {
            return 65U;
        }

        if (value <= 3600U)
        {
            return 80U;
        }

        if (value <= 3900U)
        {
            return 90U;
        }

        return 100U;
    }

    uint8_t LightSensor::GetStableBrightnessPercent()
    {
        return StableBrightness;
    }
}