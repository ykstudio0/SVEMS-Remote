#include <Arduino.h>

#include "Config.h"

#include "Display.h"
#include "DisplayModel.h"
#include "IRenderTarget.h"
#include "DisplayRenderer.h"
#include "LGFX_Config.h"
#include "TFTRenderTarget.h"

#include "WiFiService.h"
#include "RemoteTelemetryService.h"
#include "DisplayPages.h"
#include "DisplayModelBuilder.h"
#include "PageManager.h"
#include "Pins.h"
#include "ButtonManager.h"
#include <Wire.h>
#include "TouchManager.h"
#include "Localization.h"

SVEMS::Remote::TouchManager touchManager;

namespace
{
    LGFX_SVEMS display;

    TFTRenderTarget tftTarget(
        display
    );

    DisplayModel::Model displayModel;

    DisplayRenderer::Renderer displayRenderer;

    PageManager::Manager pageManager;

    bool forceDisplayRefresh =
    false;
}


void setup()
{
    Serial.begin(
        115200
    );

    delay(
        5000
    );

    Serial.println();
    Serial.println(
        "================================"
    );
    Serial.println(
        "SVEMS Remote Display"
    );
    Serial.println(
        "================================"
    );

    //---------------------------------------------------------
    // I2C
    //---------------------------------------------------------
    Wire.begin(8, 9);

    if (!touchManager.Begin())
    {
        Serial.println(
            "[TOUCH] Manager Begin failed"
        );
    }
    else
    {
        Serial.println(
            "[TOUCH] Manager Ready"
        );
    }

    //---------------------------------------------------------
    // LCD
    //---------------------------------------------------------

    display.init();

    display.setRotation(
        3
    );

    Serial.println(
        "[I2C] Scanning..."
    );

    for (uint8_t address = 1U;
        address < 127U;
        ++address)
    {
        Wire.beginTransmission(
            address
        );

        if (Wire.endTransmission() == 0U)
        {
            Serial.printf(
                "[I2C] Found 0x%02X\n",
                address
            );
        }
    }
    display.setBrightness(
        180
    );

    display.fillScreen(
        TFT_BLACK
    );

    //---------------------------------------------------------
    // Display Framework
    //---------------------------------------------------------
    Display::Begin();

    if (!displayRenderer.Begin(
            tftTarget))
    {
        Serial.println(
            "DisplayRenderer Begin failed"
        );

        return;
    }

    Serial.println(
        "DisplayRenderer Ready"
    );

    pageManager.Begin();

    //---------------------------------------------------------
    // Wi-Fi
    //---------------------------------------------------------

    WiFiService::Begin();

    ButtonManager::Begin();

    // Localization::SetLanguage(
    //     Localization::Language::Korean);
}


void loop()
{
    WiFiService::Update();

    static bool lastWiFiSetupMode =
        false;

    const bool wifiSetupMode =
        WiFiService::IsSetupMode();

    if (wifiSetupMode !=
        lastWiFiSetupMode)
    {
        lastWiFiSetupMode =
            wifiSetupMode;

        displayRenderer.SetWiFiSetupMode(
            wifiSetupMode
        );

        forceDisplayRefresh =
            true;
    }

    RemoteTelemetryService::Update();

    ButtonManager::Update();

    const ButtonManager::Button button =
        ButtonManager::GetPressed();

    switch (button)
    {   
        case ButtonManager::Button::Previous:
            pageManager.Previous();

            forceDisplayRefresh =
                true;
            break;

        case ButtonManager::Button::Next:
            pageManager.Next();

            forceDisplayRefresh =
                true;
            break;

        case ButtonManager::Button::Home:
            pageManager.SetPage(
                DisplayPages::Page::Overview
            );

            forceDisplayRefresh =
                true;
            break;

        case ButtonManager::Button::HomeLong:
        {
            const uint8_t subPageCount =
                DisplayPages::GetSubPageCount(
                    pageManager.Current()
                );

            if (subPageCount > 1U)
            {
                pageManager.NextSubPage();

                forceDisplayRefresh =
                    true;
            }

            break;
        }

        default:
            break;
    }

    const uint32_t now =
        millis();

    static uint32_t lastDisplayUpdate =
        0U;

    if (
        forceDisplayRefresh ||
        now - lastDisplayUpdate >=
            1000UL
    )
    {
        lastDisplayUpdate =
            now;

        forceDisplayRefresh =
            false;
            
        DisplayModelBuilder::Build(
            displayModel
        );

        displayRenderer.RenderPage(
            pageManager.Current(),
            pageManager.SubPage(),
            displayModel
        );
    }

    touchManager.Update();

    const SVEMS::Remote::TouchManager::Action
        touchAction =
            touchManager.GetAction();

    switch (touchAction)
    {
        case SVEMS::Remote::TouchManager::Action::WiFiSetup:
        {
            if (
                pageManager.Current() ==
                DisplayPages::Page::System
            )
            {
                Serial.println(
                    "[UI] WiFi Setup"
                );

                displayRenderer.SetWiFiSetupConfirm(
                    true
                );

                touchManager.SetWiFiSetupConfirm(
                    true
                );

                forceDisplayRefresh =
                    true;
            }

            break;
        }
        
        case SVEMS::Remote::TouchManager::Action::Cancel:
        {
            Serial.println(
                "[UI] WiFi Setup Cancel"
            );

            displayRenderer.SetWiFiSetupConfirm(
                false
            );

            touchManager.SetWiFiSetupConfirm(
                false
            );

            forceDisplayRefresh =
                true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::Confirm:
        {
            Serial.println(
                "[UI] WiFi Setup Confirm"
            );

            displayRenderer.SetWiFiSetupConfirm(
                false
            );

            touchManager.SetWiFiSetupConfirm(
                false
            );

            const bool started =
                WiFiService::StartSetupMode();

            Serial.println(
                started
                    ? "[UI] WiFi Setup AP OK"
                    : "[UI] WiFi Setup AP FAILED"
            );

            if (started)
            {
                displayRenderer.SetWiFiSetupMode(
                    true
                );

                forceDisplayRefresh =
                    true;
            }

            break;
        }

        case SVEMS::Remote::TouchManager::Action::Previous:
            pageManager.Previous();

            forceDisplayRefresh =
                true;
            break;

        case SVEMS::Remote::TouchManager::Action::Next:
            pageManager.Next();

            forceDisplayRefresh =
                true;
            break;

        case SVEMS::Remote::TouchManager::Action::Content:
        {
            const uint8_t subPageCount =
                DisplayPages::GetSubPageCount(
                    pageManager.Current()
                );

            if (subPageCount > 1U)
            {
                pageManager.NextSubPage();

                forceDisplayRefresh =
                    true;
            }

            break;
        }

        default:
            break;
    }
}