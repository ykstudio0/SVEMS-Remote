//-------------------------------------------------------------
// File : TouchManager.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-17
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Remote Display Touch Input Manager
//-------------------------------------------------------------

#pragma once

#include <Arduino.h>

#include "TouchDevice.h"

namespace SVEMS::Remote
{
    class TouchManager
    {
    public:
        enum class Action : uint8_t
        {
            None = 0U,
            Previous,
            Next,
            Content,
            WiFiSetup,
            Cancel,
            Confirm
        };

        bool Begin();

        void Update();

        Action GetAction();

        void SetWiFiSetupConfirm(
            bool active);

    private:
        Action DetermineAction(
            uint16_t x,
            uint16_t y) const;

        void ConvertCoordinates(
            uint16_t rawX,
            uint16_t rawY,
            uint16_t& screenX,
            uint16_t& screenY) const;

        SVEMS::Device::TouchDevice m_device;

        Action m_action =
            Action::None;

        bool m_wasTouched =
            false;

        bool m_wifiSetupConfirm =
            false;
    };
}