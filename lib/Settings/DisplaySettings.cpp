//-------------------------------------------------------------
// File : DisplaySettings.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-26
// Project : SVEMS Remote
// Version : 0.1.1
// Description : Language Setting
//-------------------------------------------------------------

#include "DisplaySettings.h"

#include <Preferences.h>

namespace SVEMS
{
    namespace Remote
    {
        namespace DisplaySettings
        {
            namespace
            {
                constexpr const char* NVS_NAMESPACE =
                    "display";

                constexpr const char* NVS_KEY_LANGUAGE =
                    "language";
            }

            //-------------------------------------------------
            // LoadLanguage
            //-------------------------------------------------

            Localization::Language LoadLanguage()
            {
                Preferences preferences;

                preferences.begin(
                    NVS_NAMESPACE,
                    true
                );

                const uint8_t value =
                    preferences.getUChar(
                        NVS_KEY_LANGUAGE,
                        static_cast<uint8_t>(
                            Localization::Language::English
                        )
                    );

                preferences.end();

                if (
                    value ==
                    static_cast<uint8_t>(
                        Localization::Language::Korean
                    )
                )
                {
                    return Localization::Language::Korean;
                }

                return Localization::Language::English;
            }

            //-------------------------------------------------
            // SaveLanguage
            //-------------------------------------------------

            bool SaveLanguage(
                Localization::Language language
            )
            {
                Preferences preferences;

                if (
                    !preferences.begin(
                        NVS_NAMESPACE,
                        false
                    )
                )
                {
                    return false;
                }

                const size_t written =
                    preferences.putUChar(
                        NVS_KEY_LANGUAGE,
                        static_cast<uint8_t>(language)
                    );

                preferences.end();

                return written == sizeof(uint8_t);
            }
        }
    }
}