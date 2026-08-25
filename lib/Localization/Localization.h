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

    struct Text
    {
        const char* english;
        const char* korean;
    };

    const char* Get(
        const Text& text,
        Language language);

    extern const Text Battery;
    extern const Text BMSTemp;
    extern const Text ExtTemp;
    extern const Text Capacity;
    extern const Text Remaining;
    extern const Text SOC;
    extern const Text Runtime;

    Language GetLanguage();
    void SetLanguage(
        Language language);

    const char* Get(
        const Text& text,
        Language language);

    const char* Get(
        const Text& text);
}