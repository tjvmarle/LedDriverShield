#include "FastLED.h"
#include <Arduino.h>

#include "led_constants.hpp"
#include "led_strip.hpp"

#include "shield_crest.hpp"

#include "avr8-stub.h"

void setup()
{
    // debug_init();
    randomSeed(0); // TODO: find something for this.
}

void loop()
{
    ShieldCrest zelda_shield{};
    while (1)
    {
        zelda_shield.Display();
    }
}