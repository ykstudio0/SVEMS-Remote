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
#include "Config.h"

namespace
{
    constexpr uint32_t LONG_PRESS_MS =
        1000UL;

    constexpr uint32_t DEBOUNCE_MS = 
        30UL;

    struct ButtonState
    {
        uint8_t pin;

        bool rawState;
        bool stableState;

        uint32_t lastChangeTime;

        uint32_t pressedTime;
        bool longReported;
    };

    ButtonState g_previous =
    {
        PIN_BUTTON_PREV,
        HIGH,
        HIGH,
        0UL,
        0UL,
        false
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
        ButtonManager::Button id,
        bool allowLongPress = false)
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

        //---------------------------------------------------------
        // Stable state changed
        //---------------------------------------------------------

        if (button.stableState !=
            button.rawState)
        {
            button.stableState =
                button.rawState;

            // Pressed
            if (button.stableState == LOW)
            {
                button.pressedTime =
                    now;

                button.longReported =
                    false;

                // Long press를 사용하지 않는 버튼은
                // 기존처럼 즉시 클릭 처리
                if (!allowLongPress)
                {
                    g_pressed =
                        id;
                }
            }
            // Released
            else
            {
                // Home이 long press가 아니었다면
                // release 시 short click 발생
                if (allowLongPress &&
                    !button.longReported)
                {
                    g_pressed =
                        id;
                }
            }
        }

        //---------------------------------------------------------
        // Long Press
        //---------------------------------------------------------

        if (allowLongPress &&
            button.stableState == LOW &&
            !button.longReported &&
            now - button.pressedTime >=
                LONG_PRESS_MS)
        {
            button.longReported =
                true;

            g_pressed =
                ButtonManager::Button::HomeLong;
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
            Button::Home,
            true);
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