#include "shield_crest.hpp"
#include "led_constants.hpp"

#include "FastLED.h"

ShieldCrest::ShieldCrest() : led_strip{LedStrip{NUM_LEDS}}
{
    // Boot visuals to detect resets of the code.
    led_strip.rgb_list[0] = {0U, 0U, 255U};
    led_strip.rgb_list[1] = {0U, 0U, 255U};
    led_strip.rgb_list[2] = {0U, 0U, 255U};
    led_strip.Show();
    delay(500);

    led_strip.Clear(true);

    currPattern = new SmoothRainbow{led_strip};
    // currPattern = new DemoPattern{led_strip};
}

void ShieldCrest::Display()
{
    if (runInit)
    {
        currPattern->Init();
        runInit = false;
    }
    currPattern->Play();
}

void ShieldCrest::addPattern(Pattern* newPattern)
{
    // Nothing for now. Maybe this is not the best approach. The runtime doesn't need to be that dynamic.
}