#pragma once

#include <Arduino.h>

//! @brief Collection of the different parts of the crest.
enum CrestParts
{
    TopTriangle,
    LeftTriangle,
    RightTriangle,
    LeftWing,
    RightWing,
    CentreBody,
    LeftClaw,
    RightClaw,
    All
};

//! @brief Representation for a single part of the crest. Each part has a start and end index for the LEDs.
struct MappedPart
{
    CrestParts part{};
    uint8_t start_range{};
    uint8_t end_range{};

    MappedPart(CrestParts crest_part, uint8_t start_led, uint8_t end_led);
    MappedPart(){};
};

//! @brief Representation of the crest based with its different parts.
struct MappedShield
{
    MappedPart PartsList[9U];
    MappedShield();

    const MappedPart& getPart(CrestParts part);
};