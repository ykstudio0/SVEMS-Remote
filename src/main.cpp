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
        1000
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
    // LCD
    //---------------------------------------------------------

    display.init();

    display.setRotation(
        3
    );

    display.setBrightness(
        180
    );

    display.fillScreen(
        TFT_BLACK
    );

    //---------------------------------------------------------
    // Display Framework
    //---------------------------------------------------------

    pinMode(
        PIN_BUTTON_PREV,
        INPUT_PULLUP
    );

    pinMode(
        PIN_BUTTON_NEXT,
        INPUT_PULLUP
    );

    pinMode(
        PIN_BUTTON_HOME,
        INPUT_PULLUP
    );

    Serial.println(
        "[BUTTON] Test Ready"
    );

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
}


void loop()
{
    WiFiService::Update();

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
}