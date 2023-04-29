#include "FastLED.h"
#include <Arduino.h>

#include "led_constants.hpp"
#include "led_strip.hpp"

#include "shield_crest.hpp"

#include "avr8-stub.h"

void setup()
{
    // Serial.begin(9600);
    // TODO: find something for this. Get webserver online, read NTP
    random16_set_seed(12345);
}

void loop()
{
    ShieldCrest zelda_shield{};
    while (1)
    {
        zelda_shield.Display();
        // TODO: move to ESP-01, add OTA, add webserver
    }
}