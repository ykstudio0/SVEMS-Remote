#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>

class WiFiService
{
public:
    static void Begin();

    static void Update();

    static bool IsConnected();
};

#endif