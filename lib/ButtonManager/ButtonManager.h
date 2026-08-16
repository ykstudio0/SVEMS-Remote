// Version : 0.1.0
// Description : Physical Button Input Manager
//-------------------------------------------------------------

#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>

namespace ButtonManager
{
    enum class Button : uint8_t
    {
        None = 0U,
        Previous,
        Next,
        Home
    };

    void Begin();

    void Update();

    Button GetPressed();
}

#endif