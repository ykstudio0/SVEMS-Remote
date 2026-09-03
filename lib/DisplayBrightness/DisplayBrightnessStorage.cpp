//-------------------------------------------------------------
// File : DisplayBrightnessStorage.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-03
// Project : SVEMS Remote Display
// Version : 0.1.3
// Description : Display brightness storage management for SVEMS Remote Display
//-------------------------------------------------------------

#include "DisplayBrightnessStorage.h"

#include <Preferences.h>

namespace
{
    constexpr const char* NVS_NAMESPACE =
        "remote_disp";

    constexpr const char* KEY_INITIALIZED =
        "initialized";

    constexpr const char* KEY_MODE =
        "mode";

    constexpr const char* KEY_MANUAL =
        "manual";
}

namespace SVEMS::Remote::Display
{
    bool DisplayBrightnessStorage::Load(
        BrightnessSettings& settings)
    {
        Preferences preferences;

        if (!preferences.begin(
                NVS_NAMESPACE,
                true))
        {
            return false;
        }

        const bool initialized =
            preferences.getBool(
                KEY_INITIALIZED,
                false);

        if (!initialized)
        {
            preferences.end();
            return false;
        }

        uint8_t mode =
            preferences.getUChar(
                KEY_MODE,
                static_cast<uint8_t>(
                    BrightnessMode::Manual));

        uint8_t manualPercent =
            preferences.getUChar(
                KEY_MANUAL,
                70U);

        preferences.end();

        if (mode >
            static_cast<uint8_t>(
                BrightnessMode::Auto))
        {
            mode =
                static_cast<uint8_t>(
                    BrightnessMode::Manual);
        }

        if (manualPercent > 100U)
        {
            manualPercent = 100U;
        }

        settings.mode =
            static_cast<BrightnessMode>(mode);

        settings.manualPercent =
            manualPercent;

        return true;
    }

    bool DisplayBrightnessStorage::Save(
        const BrightnessSettings& settings)
    {
        Preferences preferences;

        if (!preferences.begin(
                NVS_NAMESPACE,
                false))
        {
            return false;
        }

        uint8_t manualPercent =
            settings.manualPercent;

        if (manualPercent > 100U)
        {
            manualPercent = 100U;
        }

        preferences.putUChar(
            KEY_MODE,
            static_cast<uint8_t>(
                settings.mode));

        preferences.putUChar(
            KEY_MANUAL,
            manualPercent);

        preferences.putBool(
            KEY_INITIALIZED,
            true);

        preferences.end();

        return true;
    }
}