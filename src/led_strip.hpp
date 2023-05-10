#pragma once

#include "FastLED.h"
#include "crest_mapping.hpp"
#include <Arduino.h>

#include <NeoPixelBus.h>

//! @brief  Basic struct with color levels for a single LED.
struct Neopixel
{
    constexpr Neopixel(uint8_t r, uint8_t g, uint8_t b) : red{r}, green{g}, blue{b} {};
    constexpr Neopixel(){};
    uint8_t red{0U};
    uint8_t green{0U};
    uint8_t blue{0U};

    // FIXME: all the implicit type conversions here.
    Neopixel operator*(double fraction) const
    {
        uint8_t red = constrain(this->red * fraction, 0, 255U);
        uint8_t green = constrain(this->green * fraction, 0, 255U);
        uint8_t blue = constrain(this->blue * fraction, 0, 255U);
        return {red, green, blue};
    }

    Neopixel operator+(const Neopixel& other) const
    {

        uint8_t red = constrain(this->red + other.red, 0, 255U);
        uint8_t green = constrain(this->green + other.green, 0, 255U);
        uint8_t blue = constrain(this->blue + other.blue, 0, 255U);
        return {red, green, blue};
    }

    bool operator==(const Neopixel& other) const
    {

        return (red == other.red) && (green == other.green) && (blue == other.blue);
    }
};

//! @brief   Abstraction for controlling an LED-strip
class LedStrip
{
  private:
    const uint8_t num_leds;
    NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1Ws2812xMethod> new_actual_strip;

    //! @brief Normalize the entire LED-strip so their combined output won't blow up the power supply.
    void clampStrip();

  public:
    //! @brief Constructor
    //! @param[in]  led_count   Number of LEDs on the LED strip.
    LedStrip(uint8_t led_count);

    uint8_t getLedCount();

    //! @brief This array can be modified for visual effects. It will be copied to the inner actual_led_strip on a
    //!        Show(). This is mostly a wrapper for CRGB, to prevent unclamped Show() of the Ledstrips.
    Neopixel* rgb_list;

    //! @brief Light all LEDs according to their latest settings. Their values will be limited by the maximum available
    //!        power from the supply.
    void Show();

    //! @brief Clear some or all LED values.
    //! @param[in]  show  Blacks out de LEDs if true. Otherwise only only wipes the values, but won't show the changes.
    void Clear(bool show = false);                             //! Clear all values
    void Clear(uint8_t start, uint8_t end, bool show = false); //! Clear a specific range, including end
    void Clear(MappedPart part, bool show = false);            //! Clear a specific part
};