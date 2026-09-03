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
    // Header
    //---------------------------------------------------------

    constexpr uint16_t HEADER_BOTTOM =
        39U;

    constexpr uint16_t HEADER_STATUS_LEFT =
        290U;

    constexpr uint16_t HEADER_STATUS_RIGHT =
        319U;

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

        m_wifiSetupConfirm =
            false;

        return m_device.Begin();
    }


    //---------------------------------------------------------
    // Update
    //---------------------------------------------------------

    void TouchManager::Update(
        DisplayPages::Page page)
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
                screenY,
                page);
    }

    void TouchManager::SetSettingsMenu(
        bool active)
    {
        m_settingsMenu =
            active;
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
        uint16_t y,
        DisplayPages::Page page) const
    {
        //-------------------------------------------------
        // Display Settings
        //-------------------------------------------------
        if (m_displaySettingsMode)
        {
            // mode
            if (
                y >= 50U &&
                y <= 90U
            )
            {
                return Action::BrightnessModeToggle;
            }

            // - button
            if (
                x >= 40U &&
                x <= 120U &&
                y >= 120U &&
                y <= 165U
            )
            {
                return Action::BrightnessDecrease;
            }

            // + button
            if (
                x >= 200U &&
                x <= 280U &&
                y >= 120U &&
                y <= 165U
            )
            {
                return Action::BrightnessIncrease;
            }

            // SAVE
            if (
                x >= 40U &&
                x <= 140U &&
                y >= 190U &&
                y <= 230U
            )
            {
                return Action::DisplaySave;
            }

            // CANCEL
            if (
                x >= 180U &&
                x <= 280U &&
                y >= 190U &&
                y <= 230U
            )
            {
                return Action::DisplayCancel;
            }

            return Action::None;
        }
        
        //-------------------------------------------------
        // Settings Menu
        //-------------------------------------------------
        if (m_settingsMenu)
        {   
            if (
                y <= HEADER_BOTTOM &&
                x >= HEADER_STATUS_LEFT &&
                x <= HEADER_STATUS_RIGHT
            )
            {
                return Action::SettingsClose;
            }

            //-------------------------------------------------
            // WiFi Setup
            //-------------------------------------------------

            if (
                y >= 45U &&
                y <= 78U
            )
            {
                return Action::WiFiSetup;
            }

            //-------------------------------------------------
            // English
            //-------------------------------------------------

            if (
                y >= 79U &&
                y <= 112U
            )
            {
                return Action::English;
            }

            //-------------------------------------------------
            // Korean
            //-------------------------------------------------

            if (
                y >= 113U &&
                y <= 146U
            )
            {
                return Action::Korean;
            }

            //-------------------------------------------------
            // Display
            //-------------------------------------------------

            if (
                y >= 147U &&
                y <= 180U
            )
            {
                return Action::DisplaySettings;
            }

            //-------------------------------------------------
            // Data Source
            //-------------------------------------------------

            if (
                y >= 181U &&
                y <= 220U
            )
            {
                if (x < 160U)
                {
                    return Action::Main;
                }

                return Action::TestMain;
            }

            return Action::None;
        }

        //-----------------------------------------------------
        // WiFi Setup Confirm
        //-----------------------------------------------------
        if (m_wifiSetupConfirm)
        {
            if (y >= 135U &&
                y <= 162U)
            {
                if (x >= 70U &&
                    x <= 144U)
                {
                    return Action::Cancel;
                }

                if (x >= 175U &&
                    x <= 249U)
                {
                    return Action::Confirm;
                }
            }

            return Action::None;
        }

        //-----------------------------------------------------
        // WiFi Setup Mode
        //-----------------------------------------------------

        if (m_wifiSetupMode)
        {
            if (
                x >= 80U &&
                x <= 240U &&
                y >= 190U &&
                y <= 239U
            )
            {
                return
                    Action::WiFiSetupModeCancel;
            }

            return
                Action::None;
        }

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

        if (y <= HEADER_BOTTOM)
        {
            if (x >= HEADER_STATUS_LEFT &&
                x <= HEADER_STATUS_RIGHT)
            {
                if (page == DisplayPages::Page::System)
                {
                    return Action::Settings;
                }

                return Action::WiFiSetup;
            }

            return Action::None;
        }

        return Action::None;
    }

    void TouchManager::SetWiFiSetupConfirm(
        bool active)
    {
        m_wifiSetupConfirm =
            active;
    }

    void TouchManager::SetWiFiSetupMode(
        bool active)
    {
        m_wifiSetupMode =
            active;
    }
}