//-------------------------------------------------------------
// File : ButtonManager.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-16
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Physical Button Input Manager
//-------------------------------------------------------------

#include "ButtonManager.h"
#include "Pins.h"

namespace
{
    constexpr uint32_t DEBOUNCE_MS =
        30UL;

    struct ButtonState
    {
        uint8_t pin;

        bool rawState;
        bool stableState;

        uint32_t lastChangeTime;
    };

    ButtonState g_previous =
    {
        PIN_BUTTON_PREV,
        HIGH,
        HIGH,
        0UL
    };

    ButtonState g_next =
    {
        PIN_BUTTON_NEXT,
        HIGH,
        HIGH,
        0UL
    };

    ButtonState g_home =
    {
        PIN_BUTTON_HOME,
        HIGH,
        HIGH,
        0UL
    };

    ButtonManager::Button g_pressed =
        ButtonManager::Button::None;


    void UpdateButton(
        ButtonState& button,
        ButtonManager::Button id)
    {
        const bool raw =
            digitalRead(button.pin);

        const uint32_t now =
            millis();

        if (raw != button.rawState)
        {
            button.rawState =
                raw;

            button.lastChangeTime =
                now;
        }

        if ((now - button.lastChangeTime) <
            DEBOUNCE_MS)
        {
            return;
        }

        if (button.stableState ==
            button.rawState)
        {
            return;
        }

        button.stableState =
            button.rawState;

        // INPUT_PULLUP:
        // LOW = Pressed
        if (button.stableState == LOW)
        {
            g_pressed =
                id;
        }
    }
}


namespace ButtonManager
{
    void Begin()
    {
        pinMode(
            PIN_BUTTON_PREV,
            INPUT_PULLUP);

        pinMode(
            PIN_BUTTON_NEXT,
            INPUT_PULLUP);

        pinMode(
            PIN_BUTTON_HOME,
            INPUT_PULLUP);

        g_previous.rawState =
            digitalRead(PIN_BUTTON_PREV);

        g_previous.stableState =
            g_previous.rawState;

        g_next.rawState =
            digitalRead(PIN_BUTTON_NEXT);

        g_next.stableState =
            g_next.rawState;

        g_home.rawState =
            digitalRead(PIN_BUTTON_HOME);

        g_home.stableState =
            g_home.rawState;

        g_pressed =
            Button::None;

        Serial.println(
            "[BUTTON] Manager Ready");
    }


    void Update()
    {
        UpdateButton(
            g_previous,
            Button::Previous);

        UpdateButton(
            g_next,
            Button::Next);

        UpdateButton(
            g_home,
            Button::Home);
    }


    Button GetPressed()
    {
        const Button result =
            g_pressed;

        g_pressed =
            Button::None;

        return result;
    }
}