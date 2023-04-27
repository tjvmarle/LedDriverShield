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

SmoothRainbow::SmoothRainbow(LedStrip& leds) : Pattern(leds), colorA{NeoColor::red}, colorB{NeoColor::blue}
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
        uint16_t cyclePosition{to_u16(round(to_double(ledNr) * cycleFraction) + currCycleStep) % cycleSize};
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

    // TODO: Get rid of this duplication.
    constexpr auto quarter{to_u16(cycleSize / 4.0)};
    constexpr auto threeQuarter{to_u16(3.0 * cycleSize / 4.0)};
    static_assert(quarter + stepSize < threeQuarter,
                  "SmoothRainbow::stepSize too big, quarter and threeQuarter will overlap");

    Neopixel localA{colorA}, localB{colorB};
    using CT = ColorTransition;
    // FIXME: there seems to be a single frame where a part of the left wing blinks in a different color
    // Seems to occur every 1.5 cycle.
    if (switchingColor != CT::None)
    {
        // One of the colors has to be averaged based on their progression in the switching cycle.
        const bool doA{switchingColor == CT::A};
        auto& changingColor{doA ? colorA : colorB};

        // A switches from quarter, B from threeQuarter.
        const auto& deltapoint{doA ? quarter : threeQuarter};
        auto progression{to_double(currCycleStep + (currCycleStep < deltapoint ? cycleSize : 0U) - deltapoint) /
                         to_double(cycleSize)};
        progression *= 1.5;

        if (progression >= 1.0)
        {
            changingColor = nextColor;
            switchingColor = CT::None;
        }
        else
        {
            const auto mixColor{nextColor * progression + changingColor * (1.0 - progression)};
            if (doA)
            {
                localA = mixColor;
            }
            else
            {
                localB = mixColor;
            }
        }
    }

    for (uint8_t ledNr{0U}; ledNr < C_LED_COUNT; ledNr++)
    {
        const auto syncOffset{(ledNr + syncOffsetList[part]) % C_LED_COUNT};
        led_strip.rgb_list[syncOffset + currPart.start_range] =
            localA * colorRatio(ledNr, true) + localB * colorRatio(ledNr);
    }
}

void SmoothRainbow::Play()
{
    // TODO: measure performance. Millis between cycles. It's getting slow now.
    constexpr CrestParts partList[]{LeftWing /*, RightWing, CentreBody, LeftClaw, RightClaw*/};

    // TODO: implement seperate handler for the triangles
    for (const auto part : partList)
    {
        newRainbow(part);
    }

    led_strip.Show();
    currCycleStep = (currCycleStep + stepSize) % cycleSize;
    // delay(2U);

    constexpr auto quarter{to_u16(cycleSize / 4.0)};
    constexpr auto threeQuarter{to_u16(3.0 * cycleSize / 4.0)};
    static_assert(quarter + stepSize < threeQuarter,
                  "SmoothRainbow::stepSize too big, quarter and threeQuarter will overlap");

    // Change one of the colors if they reach (close to) zero.
    if (switchingColor != ColorTransition::None ||
        (currCycleStep != constrain(currCycleStep, quarter, quarter + stepSize - 1U)) &&
            (currCycleStep != constrain(currCycleStep, threeQuarter, threeQuarter + stepSize - 1U)))
    {
        return;
    }

    // TODO: Write a blink function for debugging purposes

    const auto giveNextColor{[&]() {
        auto colorCandidate{NeoColor::ColorList[random8(6U)]};
        while (colorCandidate == colorA || colorCandidate == colorB)
        {
            colorCandidate = NeoColor::ColorList[random8(6U)];
        }
        return colorCandidate;
    }};

    // Only switch colors if A wasn't switched this cycle and B wasn't during the previous one.
    const auto shouldSwitch{[&](bool otherSwitch, bool& thisSwitch) {
        // If the other color switched this cycle this one won't.
        if (otherSwitch)
        {
            return false;
        }

        // If this color switched in the previous cycle we also skip, but reset the flag.
        if (thisSwitch)
        {
            thisSwitch = false;
            return false;
        }

        return true;
    }};

    if (currCycleStep >= threeQuarter)
    {
        // Should be just on or past 3/4.
        if (!shouldSwitch(aSwitched, bSwitched))
        {
            return;
        }

        switchingColor = ColorTransition::B;
        bSwitched = true;
    }
    else
    {
        // Should be just on or past 1/4.
        if (!shouldSwitch(bSwitched, aSwitched))
        {
            return;
        }

        switchingColor = ColorTransition::A;
        aSwitched = true;
    }
    nextColor = giveNextColor();
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