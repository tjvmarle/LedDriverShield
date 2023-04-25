#include "pattern.hpp"
#include "led_strip.hpp"

#define to_double(x) static_cast<double>(x)
#define to_u8(x) static_cast<uint8_t>(x)
#define to_u16(x) static_cast<uint16_t>(x)

Pattern::Pattern(LedStrip& leds) : led_strip{leds}, num_leds{leds.getLedCount()}
{
}

void Pattern::SetSpeed(uint8_t speed)
{
    this->speed = speed;
}

SmoothRainbow::SmoothRainbow(LedStrip& leds) : Pattern(leds)
{
}

void SmoothRainbow::Init()
{
    // Nothing for now.
}

void SmoothRainbow::newRainbow(CrestParts part)
{
    const auto& currPart{shield.getPart(part)};
    const uint8_t C_LED_COUNT{to_u8(currPart.end_range - currPart.start_range + 1U)};
    const auto cycleFraction{to_double(cycleSize) / to_double(C_LED_COUNT)};
    const auto piFraction{2.0 * M_PI / to_double(cycleSize)};

    // LED 0 will start at the 50/50 color mix.
    auto colorRatio{[&](const uint8_t ledNr, const bool shiftHalf = false) {
        uint16_t cyclePosition{to_u16(round(to_double(ledNr) * cycleFraction) + currCycleStep)};
        return sin(to_double(cyclePosition) * piFraction + (shiftHalf ? M_PI : 0.0)) / 2.0 + 0.5;
    }};

    // Offsets to position the first LED more-or-less at a top-right position
    constexpr uint8_t syncOffsetList[9U] = {
        [0] = 0U, // These are only here for compiling purposes
        [1] = 0U,
        [2] = 0U,
        [CrestParts::LeftWing] = 7U,
        [CrestParts::RightWing] = 50U,
        [CrestParts::CentreBody] = 9U,
        [CrestParts::LeftClaw] = 3U,
        [CrestParts::RightClaw] = 8U,
        [8] = 0U,
    }; //! WARN: This can easily crash. Only use the enum-indexed values.

    auto colorA{red}, colorB{blue};
    for (uint8_t ledNr{0U}; ledNr < C_LED_COUNT; ledNr++)
    {
        const auto syncOffset{(ledNr + syncOffsetList[part]) % C_LED_COUNT};
        led_strip.rgb_list[syncOffset + currPart.start_range] =
            colorA * colorRatio(ledNr) + colorB * colorRatio(ledNr, true);
    }

    // TODO: Change the color after at least a full cycle, but only when that color reaches zero. This means each color
    // lives for 1.5 cycle.
}

void SmoothRainbow::Play()
{
    constexpr CrestParts partList[]{LeftWing, RightWing, CentreBody, LeftClaw, RightClaw};

    // TODO: implement seperate handler for the triangles
    for (const auto part : partList)
    {
        newRainbow(part);
    }

    led_strip.Show();
    currCycleStep += 8U;
    currCycleStep = currCycleStep % cycleSize;

    delay(5U);
}

DemoPattern::DemoPattern(LedStrip& leds) : Pattern(leds)
{
}

void DemoPattern::Init()
{
}

void DemoPattern::Play()
{
    led_strip.Clear();
    const auto part{shield.PartsList[CrestParts::CentreBody]};

    for (uint8_t led_index{part.start_range}; led_index <= part.end_range; led_index++)
    {
        auto& curr_led{led_strip.rgb_list[led_index]};

        uint8_t max{100U};
        Neopixel clr{max, max, max};

        curr_led.red = clr.red;
        curr_led.green = clr.green;
        curr_led.blue = clr.blue;

        led_strip.Show();
        delay(100U);
    }
}