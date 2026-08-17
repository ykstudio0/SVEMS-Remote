//-------------------------------------------------------------
// File : TouchDevice.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-17
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : FT6336G capacitive touch controller driver
//-------------------------------------------------------------

#pragma once

#include <Arduino.h>

namespace SVEMS::Device
{
    class TouchDevice
    {
    public:
        bool Begin();

        bool Update();

        bool IsReady() const;

        bool IsTouched() const;

        bool ReadPoint(
            uint16_t& x,
            uint16_t& y) const;

    private:
        bool ResetController();

        bool ReadRegister(
            uint8_t registerAddress,
            uint8_t& value);

        bool ReadRegisters(
            uint8_t registerAddress,
            uint8_t* buffer,
            size_t length);

        bool m_ready = false;
        bool m_touched = false;

        uint16_t m_x = 0U;
        uint16_t m_y = 0U;
    };
}