#include "led_strip.hpp"
#include "led_constants.hpp"

LedStrip::LedStrip(uint8_t led_count) : num_leds{led_count}
{
    actual_led_strip = new CRGB[led_count]; //! Only used on a call to Show().
    rgb_list = new Neopixel[led_count];     //! Publicly available for modification.

    FastLED.addLeds<WS2812, LED_PIN, GRB>(actual_led_strip, NUM_LEDS);
}

uint8_t LedStrip::getLedCount()
{
    return num_leds;
}

void LedStrip::clampStrip()
{
    // Accumulate all color levels and normalize against a maximum value.
    uint32_t totalColorLevel{0U};

    for (uint8_t led_index{0U}; led_index < num_leds; led_index++)
    {
        auto& curr_led{rgb_list[led_index]};

        totalColorLevel += curr_led.red;
        totalColorLevel += curr_led.green;
        totalColorLevel += curr_led.blue;
    }

    // Only adjust if threshold is exceeded, otherwise just leave as is.
    if (totalColorLevel <= MAX_AVAIL_LVLS)
    {
        for (uint8_t led_index{0U}; led_index < num_leds; led_index++)
        {
            // Never save the normalized values to the Neopixel list, otherwise repeated normalizations will keep
            // dimming any unmodified LEDs.
            auto curr_led{rgb_list[led_index]};

            auto& actual_led{actual_led_strip[led_index]};
            actual_led.red = curr_led.red;
            actual_led.green = curr_led.green;
            actual_led.blue = curr_led.blue;
        }
        return;
    }

    // Normalize all LEDs so their total is brought back below the max.
    const double ratio{MAX_AVAIL_LVLS / static_cast<double>(totalColorLevel)};
    uint32_t backupCounter{0U};
    const auto normalize{[&ratio, &backupCounter](uint8_t& led_color) {
        led_color = static_cast<uint8_t>(static_cast<double>(led_color) * ratio);
        backupCounter += led_color;
    }};

    for (uint8_t led_index{0U}; led_index < num_leds; led_index++)
    {
        // Never save the normalized values to the Neopixel list, otherwise repeated normalizations will keep dimming
        // any unmodified LEDs.
        auto curr_led{rgb_list[led_index]};
        normalize(curr_led.red);
        normalize(curr_led.green);
        normalize(curr_led.blue);

        auto& actual_led{actual_led_strip[led_index]};
        actual_led.red = curr_led.red;
        actual_led.green = curr_led.green;
        actual_led.blue = curr_led.blue;
    }

    // This should never trigger, but I also don't want to risk a blown power supply because of a simple mistake.
    if (backupCounter > MAX_AVAIL_LVLS)
    {
        // Something went horribly wrong.
        for (uint8_t led_index{0U}; led_index < num_leds; led_index++)
        {
            auto curr_led{rgb_list[led_index]};

            curr_led.red = (led_index % 3U != 0U) ? 0U : 10U;
            curr_led.green = 0U;
            curr_led.blue = 0U;
        }
    }
}

void LedStrip::Show()
{
    clampStrip();
    FastLED.show();
}

void LedStrip::Clear(uint8_t start, uint8_t end, bool show)
{
    for (uint8_t led_index{start}; led_index <= end; led_index++)
    {
        auto& led{rgb_list[led_index]};
        led.red = 0U;
        led.green = 0U;
        led.blue = 0U;
    }
    FastLED.clear(show);
}

void LedStrip::Clear(MappedPart part, bool show)
{
    this->Clear(part.start_range, part.end_range, show);
}

void LedStrip::Clear(bool show)
{
    this->Clear(0U, this->num_leds - 1U, show);
}