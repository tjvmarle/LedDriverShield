#include "led_constants.hpp"
#include "led_strip.hpp"
#include "shield_crest.hpp"
#include "wifi_creds.hpp"

#include "FastLED.h"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <Arduino.h>

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest* request)
{
    request->send(404, "text/plain", "Not found");
}

void setup()
{
    // TODO: Add mDNS
    WiFi.begin(SSID_HOME, SSID_PASS);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        // Fails after 60s. TODO: Create a warning blink.
    }
    else
    {
        // Create a "Connected blink."
    }

    server.on("/", HTTP_GET,
              [](AsyncWebServerRequest* request) { request->send(200, "text/plain", "Hello, ESP-01 is online!"); });

    server.onNotFound(notFound);

    server.begin();

    // TODO: find something for this. Get webserver online, read NTP
    random16_set_seed(12345);
}

void loop()
{
    // ShieldCrest zelda_shield{};
    // while (1)
    // {
    //     zelda_shield.Display();
    //     // TODO: move to ESP-01, add OTA, add webserver
    // }
}