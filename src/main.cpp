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
#include "DisplaySettings.h"
#include "DisplayBrightnessManager.h"

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

    Localization::SetLanguage(
        SVEMS::Remote::DisplaySettings::LoadLanguage()
    );

    Localization::SetDataSource(
        SVEMS::Remote::DisplaySettings::LoadDataSource()
    );

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

    SVEMS::Remote::Display::
        DisplayBrightnessManager::Begin();

    display.setBrightness(
        SVEMS::Remote::Display::
            DisplayBrightnessManager::GetBrightness());

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

    touchManager.Update(
        pageManager.Current());

    const SVEMS::Remote::TouchManager::Action
        touchAction =
            touchManager.GetAction();

    switch (touchAction)
    {
        case SVEMS::Remote::TouchManager::Action::DisplaySettings:
        {
            displayRenderer.SetSettingsMenu(
                false
            );

            touchManager.SetSettingsMenu(
                false
            );

            const auto& settings =
                SVEMS::Remote::Display::
                    DisplayBrightnessManager::GetSettings();

            displayRenderer.BeginDisplaySettings(
                settings.mode,
                settings.manualPercent
            );

            touchManager.SetDisplaySettingsMode(
                true
            );

            forceDisplayRefresh =
                true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::BrightnessModeToggle:
        {
            displayRenderer.ToggleBrightnessMode();

            forceDisplayRefresh = true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::BrightnessDecrease:
        {
            displayRenderer.DecreaseBrightness();

            display.setBrightness(
                displayRenderer.GetBrightnessPreview()
            );

            forceDisplayRefresh = true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::BrightnessIncrease:
        {
            displayRenderer.IncreaseBrightness();

            display.setBrightness(
                displayRenderer.GetBrightnessPreview()
            );

            forceDisplayRefresh = true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::DisplaySave:
        {
            SVEMS::Remote::Display::
                DisplayBrightnessManager::SaveSettings(
                    displayRenderer.GetBrightnessEditMode(),
                    displayRenderer.GetBrightnessEditPercent()
                );

            displayRenderer.EndDisplaySettings();

            touchManager.SetDisplaySettingsMode(
                false
            );

            forceDisplayRefresh = true;

            break;
        }
        
        case SVEMS::Remote::TouchManager::Action::DisplayCancel:
        {
            const auto& settings =
                SVEMS::Remote::Display::
                    DisplayBrightnessManager::GetSettings();

            display.setBrightness(
                SVEMS::Remote::Display::
                    DisplayBrightnessManager::
                        PercentToBrightness(
                            settings.manualPercent
                        )
            );

            displayRenderer.EndDisplaySettings();

            touchManager.SetDisplaySettingsMode(
                false
            );

            forceDisplayRefresh = true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::WiFiSetup:
        {
            if (
                pageManager.Current() ==
                DisplayPages::Page::System
            )
            {
                displayRenderer.SetSettingsMenu(
                    false
                );

                touchManager.SetSettingsMenu(
                    false
                );
                
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

        case SVEMS::Remote::TouchManager::Action::English:
        {
            Serial.println(
                "[UI] Language English"
            );

            Localization::SetLanguage(
                Localization::Language::English
            );

            SVEMS::Remote::DisplaySettings::SaveLanguage(
                Localization::Language::English
            );

            displayRenderer.SetSettingsMenu(false);
            touchManager.SetSettingsMenu(false);

            forceDisplayRefresh = true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::Korean:
        {
            Serial.println(
                "[UI] Language Korean"
            );

            Localization::SetLanguage(
                Localization::Language::Korean
            );

            SVEMS::Remote::DisplaySettings::SaveLanguage(
                Localization::Language::Korean
            );

            displayRenderer.SetSettingsMenu(false);
            touchManager.SetSettingsMenu(false);

            forceDisplayRefresh = true;

            break;
        }
        
        case SVEMS::Remote::TouchManager::Action::Main:
        {
            Serial.println(
                "[UI] Data Source MAIN"
            );

            Localization::SetDataSource(
                Localization::DataSource::Main
            );

            SVEMS::Remote::DisplaySettings::SaveDataSource(
                Localization::DataSource::Main
            );

            displayRenderer.SetSettingsMenu(
                false
            );

            touchManager.SetSettingsMenu(
                false
            );

            forceDisplayRefresh = true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::TestMain:
        {
            Serial.println(
                "[UI] Data Source TEST MAIN"
            );

            Localization::SetDataSource(
                Localization::DataSource::TestMain
            );

            SVEMS::Remote::DisplaySettings::SaveDataSource(
                Localization::DataSource::TestMain
            );

            displayRenderer.SetSettingsMenu(
                false
            );

            touchManager.SetSettingsMenu(
                false
            );

            forceDisplayRefresh = true;

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

                touchManager.SetWiFiSetupMode(
                    true);

                forceDisplayRefresh =
                    true;
            }

            break;
        }

        case SVEMS::Remote::TouchManager::Action::WiFiSetupModeCancel:
        {
            Serial.println(
                "[UI] WiFi Setup Mode Cancel"
            );

            //-------------------------------------------------
            // AP 종료 + 기존 WiFi 복귀
            //-------------------------------------------------

            WiFiService::StopSetupMode();

            //-------------------------------------------------
            // Display / Touch Setup Mode 종료
            //-------------------------------------------------

            displayRenderer.SetWiFiSetupMode(
                false
            );

            touchManager.SetWiFiSetupMode(
                false
            );

            //-------------------------------------------------
            // 정상 화면 강제 갱신
            //-------------------------------------------------

            forceDisplayRefresh =
                true;

            break;
        }

        case SVEMS::Remote::TouchManager::Action::Settings:
        {
            Serial.println(
                "[UI] Settings Menu"
            );

            displayRenderer.SetSettingsMenu(
                true
            );

            touchManager.SetSettingsMenu(
                true
            );

            forceDisplayRefresh =
                true;

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

        case SVEMS::Remote::TouchManager::Action::SettingsClose:
        {
            displayRenderer.SetSettingsMenu(
                false);

            touchManager.SetSettingsMenu(
                false);

            forceDisplayRefresh =
                true;

            break;
        }

        default:
            break;
    }

    static uint32_t lastLightPrintMs = 0U;

    const uint32_t nowMs =
        millis();

    if (
        nowMs - lastLightPrintMs >=
        1000U
    )
    {
        lastLightPrintMs =
            nowMs;

        const uint16_t lightRaw =
            analogRead(
                PIN_LIGHT_SENSOR);

        Serial.print(
            "[LIGHT] "
        );

        Serial.println(
            lightRaw
        );
    }
}