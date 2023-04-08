#include "shield_crest.hpp"
#include "led_constants.hpp"

#include "FastLED.h"

ShieldCrest::ShieldCrest() : led_strip{LedStrip{NUM_LEDS}}
{
    // Filler pattern for now.
    currPattern = new DemoPattern{led_strip};
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