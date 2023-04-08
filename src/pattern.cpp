#include "pattern.hpp"
#include "led_strip.hpp"

Pattern::Pattern(LedStrip& leds) : led_strip{leds}, num_leds{leds.getLedCount()}
{
}

void Pattern::SetSpeed(uint8_t speed)
{
    this->speed = speed;
}

SmoothRainbow::SmoothRainbow(LedStrip& leds) : Pattern(leds)
{
}

void SmoothRainbow::Init()
{
    // Start off with red and green on 0 an 10. Transition smoothly between these.
    led_strip.rgb_list[0U] = {254U, 0U, 0U};  // red
    led_strip.rgb_list[10U] = {0U, 254U, 0U}; // green

    for (uint8_t led_index{1U}; led_index < 10U; led_index++)
    {
        Neopixel clr{0U, 0U, 0U};
        clr.red = static_cast<uint8_t>(25.4 * static_cast<double>(led_index));
        clr.green = static_cast<uint8_t>(254.0 - 25.4 * static_cast<double>(led_index));
        led_strip.rgb_list[led_index] = clr;
        led_strip.rgb_list[20U - led_index] = clr;
    }
}

// Rainbow table, no not the one for cracking hashes
// {254U, 0U, 0U};   // Red
// {127U, 127U, 0U}; // Yellow
// {0U, 254U, 0U};   // Green
// {0U, 127U, 127U}; // Teal
// {0U, 0U, 254U};   // Blue
// {127U, 0U, 127U}; // Purple

// We need:
// * initialization for the pattern --> seperate method
// * Pattern progression
// ** This should be more or less independent on the amount of LEDS
// ** It would be nice if the edges of the pattern transitioned smoothly
// ** Perhaps have X amount of colors within a part and randomly cycle towards other colors.
// * A way to record the current state - per part
// * A way te resume from a recorded state - per part
void SmoothRainbow::Play()
{
    // Play some cool pattern here.
    // Preferably each part has their own rainbow-state that dances around their range.

    // First try a bit on a single part
    auto part{this->shield.PartsList[CrestParts::CentreBody]};
}

DemoPattern::DemoPattern(LedStrip& leds) : Pattern(leds)
{
}

void DemoPattern::Init()
{
}

void DemoPattern::Play()
{
    led_strip.Clear();
    const auto part{shield.PartsList[CrestParts::CentreBody]};

    for (uint8_t led_index{part.start_range}; led_index <= part.end_range; led_index++)
    {
        auto& curr_led{led_strip.rgb_list[led_index]};

        uint8_t max{100U};
        Neopixel clr{max, max, max};

        curr_led.red = clr.red;
        curr_led.green = clr.green;
        curr_led.blue = clr.blue;

        led_strip.Show();
        delay(100U);
    }
}