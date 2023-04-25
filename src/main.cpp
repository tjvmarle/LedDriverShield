#include "FastLED.h"
#include <Arduino.h>

#include "led_constants.hpp"
#include "led_strip.hpp"

#include "shield_crest.hpp"

void setup()
{
}

void loop()
{
    ShieldCrest zelda_shield{};
    while (1)
    {
        zelda_shield.Display();
    }
}