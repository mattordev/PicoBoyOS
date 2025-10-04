#pragma once

// -------------------------------
// PicoBoy OS Configuration
// -------------------------------

// -------------------------------
// Encoder (rotary knob) pins
// -------------------------------
// Pin connected to encoder channel A
constexpr int PB_ENCODER_PIN_A      = 12;
// Pin connected to encoder channel B
constexpr int PB_ENCODER_PIN_B      = 13;
// Pin connected to encoder push button
constexpr int PB_ENCODER_BUTTON_PIN = 14;

// -------------------------------
// Touchscreen SPI pin
// -------------------------------
// Chip select (T_CS) pin for the touch controller
constexpr int PB_TOUCH_CS = 22;

// -------------------------------
// TFT display SPI pin
// -------------------------------
// Chip select (TFT_CS) pin for the TFT display
constexpr int PB_TFT_CS = 17;

// -------------------------------
// Timing and debounce
// -------------------------------
// Debounce delay for button presses (ms)
constexpr unsigned long PB_DEBOUNCE_DELAY = 50;

// Interval for polling encoder position (ms)
constexpr unsigned long PB_ENCODER_INTERVAL = 10;

// Interval for checking button state (ms)
constexpr unsigned long PB_BUTTON_INTERVAL = 10;

// LED blink interval for boot sequence (ms)
constexpr unsigned long PB_BOOT_BLINK_INTERVAL = 100;

// -------------------------------
// Network defaults
// -------------------------------
// WiFi connection timeout (ms)
constexpr unsigned long PB_WIFI_TIMEOUT_MS = 20000; // 20 seconds

// -------------------------------
// Temperature sensor limits
// -------------------------------
// Minimum and maximum valid internal temperature readings (°C)
constexpr float PB_TEMP_MIN_C = -20.0;
constexpr float PB_TEMP_MAX_C = 150.0;
