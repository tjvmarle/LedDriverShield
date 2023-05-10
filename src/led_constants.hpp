#pragma once

#include <Arduino.h>

constexpr uint8_t NUM_LEDS{196U}; // Change this to match the count on the LED strip.
constexpr uint8_t LED_PIN{2U};    // Pin for the data line. Do not forget ~330Ω resistor.

// A bit of math. I'll assume the compiler optimizes most of it away.
constexpr uint32_t MAX_CURRENT_LED{60U};                          // 20mA per color.
constexpr uint32_t MAX_CURRENT_DRAWN{NUM_LEDS * MAX_CURRENT_LED}; //~12.000 mA.

//! @warning Do not exceed the current of the power supply.
constexpr uint32_t MAX_CURRENT_SUPPLY{2300U}; // 2.5A minus a small buffer. Don't forget the ESP-01 also needs power.

constexpr uint32_t MAX_LED_LVL{255u};
constexpr uint32_t LED_COLORS{3U};
constexpr uint32_t MAX_LED_LEVELS{NUM_LEDS * MAX_LED_LVL * LED_COLORS}; // 151.470

constexpr double LVLS_PER_MILLI_AMP{static_cast<double>(MAX_LED_LEVELS) / static_cast<double>(MAX_CURRENT_DRAWN)};
constexpr double MAX_AVAIL_LVLS{static_cast<double>(LVLS_PER_MILLI_AMP) * static_cast<double>(MAX_CURRENT_SUPPLY)};