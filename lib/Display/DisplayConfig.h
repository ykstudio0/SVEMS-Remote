//-------------------------------------------------------------
// File : DisplayConfig.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-09-23
// Project : SVEMS Remote Display
// Version : 0.1.3
// Description : Display Configuration
//-------------------------------------------------------------

#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <Arduino.h>

//-------------------------------------------------------------
// Display Rotation by Hardware Type
//-------------------------------------------------------------

#if defined(SVEMS_REMOTE)

constexpr uint8_t DISPLAY_ROTATION = 1U;

#elif defined(SVEMS_REMOTE_SPI)

constexpr uint8_t DISPLAY_ROTATION = 4U;

#else

#error "Remote hardware type is not defined"

#endif

#endif