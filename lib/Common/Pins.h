//-------------------------------------------------------------
// File : Pins.h
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-15
// Project : SVEMS Remote Display
// Version : 0.1.0
// Description : GPIO Pin Management
//-------------------------------------------------------------

#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// ===========================
// Physical Button Configuration
// ===========================
constexpr uint8_t PIN_BUTTON_PREV = 1;
constexpr uint8_t PIN_BUTTON_NEXT = 2;
constexpr uint8_t PIN_BUTTON_HOME = 3;

// ===========================
// TFT SPI Configuration
// ===========================
constexpr uint8_t PIN_TFT_CS   = 10;
constexpr uint8_t PIN_TFT_MOSI = 11;
constexpr uint8_t PIN_TFT_SCK  = 12;
constexpr uint8_t PIN_TFT_MISO = 13;
constexpr uint8_t PIN_TFT_DC   = 14;
constexpr uint8_t PIN_TFT_RST  = 15;
constexpr uint8_t PIN_TFT_BL   = 21;

// ===========================
// Touch Configuration
// ===========================

#if defined(SVEMS_REMOTE)

// Capacitive Touch
constexpr uint8_t PIN_CTP_RST = 6;
constexpr uint8_t PIN_CTP_INT = 7;

#elif defined(SVEMS_REMOTE_SPI)

// Resistive Touch - XPT2046
constexpr uint8_t PIN_TOUCH_CS  = 6;
constexpr uint8_t PIN_TOUCH_IRQ = 7;

constexpr uint8_t PIN_TOUCH_CLK = PIN_TFT_SCK;
constexpr uint8_t PIN_TOUCH_DIN = PIN_TFT_MOSI;
constexpr uint8_t PIN_TOUCH_DO  = PIN_TFT_MISO;

#else

#error "Remote hardware type is not defined"

#endif

// ===========================
// SD Card Configuration
// ===========================
constexpr uint8_t PIN_SD_CS = 5;

// ===========================
// Light Sensor Configuration
// ===========================
constexpr uint8_t PIN_LIGHT_SENSOR = 4;

#endif