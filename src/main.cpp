#include "FastLED.h"
#include <Arduino.h>

#include "led_constants.hpp"
#include "led_strip.hpp"

#include "shield_crest.hpp"

ShieldCrest zelda_shield{};

// TODO: Move this to its own git repo
void setup()
{
}

void loop()
{
    zelda_shield.Display();
}