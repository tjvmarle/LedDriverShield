#include "FastLED.h"
#include <Arduino.h>

#include "led_constants.hpp"
#include "led_strip.hpp"

#include "shield_crest.hpp"

#include "avr8-stub.h"

void setup()
{
    // Serial.begin(9600);
    // TODO: find something for this. Maybe read a bunch of floating GPIO inputs?
    randomSeed(0);
}

void loop()
{
    ShieldCrest zelda_shield{};
    while (1)
    {
        zelda_shield.Display();
    }
}