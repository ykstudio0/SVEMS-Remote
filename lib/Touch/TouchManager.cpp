//-------------------------------------------------------------
// File : TouchManager.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-17
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : Remote Display Touch Input Manager
//-------------------------------------------------------------

#include "TouchManager.h"

namespace
{
    //---------------------------------------------------------
    // Display
    //---------------------------------------------------------

    constexpr uint16_t SCREEN_WIDTH =
        320U;

    constexpr uint16_t SCREEN_HEIGHT =
        240U;


    //---------------------------------------------------------
    // Touch Regions
    //---------------------------------------------------------

    constexpr uint16_t CONTENT_TOP =
        40U;

    constexpr uint16_t CONTENT_BOTTOM =
        199U;

    constexpr uint16_t FOOTER_TOP =
        200U;

    constexpr uint16_t FOOTER_BOTTOM =
        239U;


    //---------------------------------------------------------
    // Footer Buttons
    //---------------------------------------------------------

    constexpr uint16_t PREVIOUS_RIGHT =
        100U;

    constexpr uint16_t NEXT_LEFT =
        220U;
}


namespace SVEMS::Remote
{
    //---------------------------------------------------------
    // Begin
    //---------------------------------------------------------

    bool TouchManager::Begin()
    {
        m_action =
            Action::None;

        m_wasTouched =
            false;

        return m_device.Begin();
    }


    //---------------------------------------------------------
    // Update
    //---------------------------------------------------------

    void TouchManager::Update()
    {
        m_action =
            Action::None;

        if (!m_device.Update())
        {
            return;
        }

        const bool touched =
            m_device.IsTouched();

        //-----------------------------------------------------
        // Released
        //-----------------------------------------------------

        if (!touched)
        {
            m_wasTouched =
                false;

            return;
        }

        //-----------------------------------------------------
        // Already processed current touch
        //-----------------------------------------------------

        if (m_wasTouched)
        {
            return;
        }

        m_wasTouched =
            true;

        //-----------------------------------------------------
        // Read raw point
        //-----------------------------------------------------

        uint16_t rawX = 0U;
        uint16_t rawY = 0U;

        if (!m_device.ReadPoint(
                rawX,
                rawY))
        {
            return;
        }

        //-----------------------------------------------------
        // Convert to display coordinates
        //-----------------------------------------------------

        uint16_t screenX = 0U;
        uint16_t screenY = 0U;

        ConvertCoordinates(
            rawX,
            rawY,
            screenX,
            screenY);

        //-----------------------------------------------------
        // Determine action
        //-----------------------------------------------------

        m_action =
            DetermineAction(
                screenX,
                screenY);
    }


    //---------------------------------------------------------
    // GetAction
    //---------------------------------------------------------

    TouchManager::Action
    TouchManager::GetAction()
    {
        const Action result =
            m_action;

        m_action =
            Action::None;

        return result;
    }


    //---------------------------------------------------------
    // ConvertCoordinates
    //---------------------------------------------------------

    void TouchManager::ConvertCoordinates(
        uint16_t rawX,
        uint16_t rawY,
        uint16_t& screenX,
        uint16_t& screenY) const
    {
        //-----------------------------------------------------
        // LCD Rotation = 3
        //
        // Raw touch:
        // X = vertical axis
        // Y = horizontal axis
        //-----------------------------------------------------

        screenX =
            rawY;

        if (screenX >= SCREEN_WIDTH)
        {
            screenX =
                SCREEN_WIDTH - 1U;
        }

        if (rawX < SCREEN_HEIGHT)
        {
            screenY =
                (SCREEN_HEIGHT - 1U) -
                rawX;
        }
        else
        {
            screenY =
                0U;
        }
    }


    //---------------------------------------------------------
    // DetermineAction
    //---------------------------------------------------------

    TouchManager::Action
    TouchManager::DetermineAction(
        uint16_t x,
        uint16_t y) const
    {
        //-----------------------------------------------------
        // Footer
        //-----------------------------------------------------

        if (y >= FOOTER_TOP &&
            y <= FOOTER_BOTTOM)
        {
            if (x <= PREVIOUS_RIGHT)
            {
                return Action::Previous;
            }

            if (x >= NEXT_LEFT)
            {
                return Action::Next;
            }

            return Action::None;
        }

        //-----------------------------------------------------
        // Content
        //-----------------------------------------------------

        if (y >= CONTENT_TOP &&
            y <= CONTENT_BOTTOM)
        {
            return Action::Content;
        }

        //-----------------------------------------------------
        // Header
        //-----------------------------------------------------

        return Action::None;
    }
}