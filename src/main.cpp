#include "led_constants.hpp"
#include "led_strip.hpp"
#include "shield_crest.hpp"
#include "wifi_creds.hpp"

#include "FastLED.h"

#include <NeoPixelBus.h>

const uint8_t dummyVal = 2; // Output pin, but ignored for Esp8266
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1Ws2812xMethod> strip(NUM_LEDS, dummyVal);

// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <ArduinoOTA.h>

// AsyncWebServer server(80);

// void notFound(AsyncWebServerRequest* request)
// {
//     request->send(404, "text/plain", "Not found");
// }

void setup()
{
    strip.Begin();
    strip.Show();

    // TODO: Add mDNS, I'm too lazy to remember IP-addresses
    // WiFi.begin(SSID_HOME, SSID_PASS);
    // if (WiFi.waitForConnectResult() != WL_CONNECTED)
    // {
    //     // Fails after 60s. TODO: Create a warning blink.
    // }
    // else
    // {
    //     // Create a "Connected blink."
    // }

    // server.on("/", HTTP_GET,
    //           [](AsyncWebServerRequest* request) { request->send(200, "text/plain", "Hello, ESP-01 is online!"); });
    // server.onNotFound(notFound);
    // server.begin();

    // TODO: find something for this. Get webserver online, read NTP
    random16_set_seed(12345);
}

void loop()
{
    ShieldCrest zelda_shield{};
    while (1)
    {
        zelda_shield.Display();
        // TODO: add webserver, add OTA
    }
}