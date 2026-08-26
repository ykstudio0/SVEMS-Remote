//-------------------------------------------------------------
// File : DisplaySettings.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-26
// Project : SVEMS Remote
// Version : 0.1.1
// Description : Language Setting
//-------------------------------------------------------------

#pragma once

#include "Localization.h"

namespace SVEMS
{
    namespace Remote
    {
        namespace DisplaySettings
        {
            Localization::Language LoadLanguage();

            bool SaveLanguage(
                Localization::Language language
            );
        }
    }
}