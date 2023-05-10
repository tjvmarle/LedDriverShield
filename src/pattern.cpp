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

Neopixel SmoothRainbow::getTransitionColor()
{
    using CT = ColorTransition;
    if (switchingColor == CT::None)
    {
        // This can be triggered, but should never be shown.
        return {};
    }
    // One of the colors has to be averaged based on their progression in the switching cycle.
    const bool doA{switchingColor == CT::A};
    auto& changingColor{doA ? colorA : colorB};

    switchProgression += stepSize;
    auto progression{to_double(switchProgression) / to_double(cycleSize)};

    if (progression >= 1.0)
    {
        changingColor = nextColor;
        switchingColor = CT::None;
        switchProgression = 0;

        return changingColor;
    }
    else
    {
        return nextColor * progression + changingColor * (1.0 - progression);
    }
}

void SmoothRainbow::setRainbowPart(CrestParts part)
{
    // TODO: Code cleanup
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
    // clang-format off
    constexpr uint8_t syncOffsetList[9U] = {
        [CrestParts::TopTriangle] = 3U,
        [CrestParts::LeftTriangle] = 5U,
        [CrestParts::RightTriangle] = 3U,
        [CrestParts::LeftWing] = 7U,
        [CrestParts::RightWing] = 50U,
        [CrestParts::CentreBody] = 9U,
        [CrestParts::LeftClaw] = 3U,
        [CrestParts::RightClaw] = 8U,
        [8] = 0U,
    }; //! WARN: This can easily crash. Only use the enum-indexed values.
    // clang-format on

    const bool switchA{switchingColor == ColorTransition::A}, switchB{switchingColor == ColorTransition::B};
    Neopixel localA{switchA ? transitionColor : colorA}, localB{switchB ? transitionColor : colorB};
    for (uint8_t ledNr{0U}; ledNr < C_LED_COUNT; ledNr++)
    {
        const auto syncOffset{(ledNr + syncOffsetList[part]) % C_LED_COUNT};
        led_strip.rgb_list[syncOffset + currPart.start_range] =
            localA * colorRatio(ledNr, true) + localB * colorRatio(ledNr);
    }
}

void SmoothRainbow::setRainbowTriangle()
{
    // We want the behavior where the 3 triangles act as a single big one. The inner led colors will be calculated as
    // averages from the other ones.
    //
    //        Outer mapping                                 Inner mapping
    //
    //           8 /\ 7 <-- Top-right is default 0, this         /\     No top-right definition required here, since
    //            /  \      syncs up all the different parts    /  \    we're just averaging already calculated colors
    //           /    \                                        /    \   .
    //        9 /______\ 6                                    /______\  .
    //                                                         4    3
    //    10 /\           /\ 5                            /\ 5        2 /\     .
    //      /  \         /  \                            /  \          /  \    Both outerleds on this side
    //     /    \       /    \                          /    \        /    \   map to innerled 2
    // 11 /______\     /______\ 4                      /______\ 0  1 /______\  .
    //     0    1       2    3
    //
    // TODO: This could easily be achieved by defining these parts as objects and overload the indexing [].

    // TODO: Cleanup. Bit of a hack and lots of duplication. It works though.
    uint8_t bigTriangleIndex[] = {[0] = 99,  [1] = 100, [2] = 115, [3] = 116, [4] = 111,  [5] = 112,
                                  [6] = 105, [7] = 106, [8] = 107, [9] = 108, [10] = 103, [11] = 104};
    constexpr uint8_t C_LED_COUNT{sizeof(bigTriangleIndex) / sizeof(bigTriangleIndex[0])};
    const auto cycleFraction{to_double(cycleSize) / to_double(C_LED_COUNT)};
    const auto piFraction{2.0 * M_PI / to_double(cycleSize)};

    auto colorRatio{[&](const uint8_t ledNr, const bool shiftHalf = false) {
        uint16_t cyclePosition{to_u16(round(to_double(ledNr) * cycleFraction) + currCycleStep) % cycleSize};
        return sin(to_double(cyclePosition) * piFraction + (shiftHalf ? M_PI : 0.0)) / 2.0 + 0.5;
    }};

    constexpr uint8_t offsetValue{7U};
    const bool switchA{switchingColor == ColorTransition::A}, switchB{switchingColor == ColorTransition::B};
    Neopixel localA{switchA ? transitionColor : colorA}, localB{switchB ? transitionColor : colorB};
    for (uint8_t ledNr{0U}; ledNr < C_LED_COUNT; ledNr++)
    {
        const uint8_t syncedLed{to_u8((ledNr + offsetValue) % C_LED_COUNT)};
        led_strip.rgb_list[bigTriangleIndex[syncedLed]] = localA * colorRatio(ledNr, true) + localB * colorRatio(ledNr);
    }

    // The inner leds are just averages of the outervalues.
    uint8_t innerLedList[] = {[0] = 101, [1] = 114, [2] = 113, [3] = 110, [4] = 109, [5] = 102};
    constexpr uint8_t C_HALF_COUNT{sizeof(innerLedList) / sizeof(innerLedList[0])};
    for (uint8_t ledNr{0U}; ledNr < C_HALF_COUNT; ledNr++)
    {
        // innerled 0 maps to outerleds 0-1, 1 to 2-3, etc.
        const auto& color1{led_strip.rgb_list[bigTriangleIndex[2U * ledNr]]};
        const auto& color2{led_strip.rgb_list[bigTriangleIndex[2U * ledNr + 1]]};
        const auto avgColor{color1 * 0.5 + color2 * 0.5};
        led_strip.rgb_list[innerLedList[ledNr]] = avgColor;
    }
}

void SmoothRainbow::Play()
{
    // TODO: measure performance. Millis between cycles. It's getting slow now.
    constexpr CrestParts partList[]{LeftWing, RightWing, CentreBody, LeftClaw, RightClaw};
    // constexpr CrestParts partList[]{RightWing}; // Debugging purposes.

    for (const auto part : partList)
    {
        setRainbowPart(part);
    }
    setRainbowTriangle();

    led_strip.Show();
    delay(5U); // TODO: Check max update frequency of the strip. ~170 fps should be a theoretical max.
    currCycleStep = (currCycleStep + stepSize) % cycleSize;

    constexpr auto quarter{to_u16(cycleSize / 4.0)};
    constexpr auto threeQuarter{to_u16(3.0 * cycleSize / 4.0)};
    static_assert(quarter + stepSize < threeQuarter,
                  "SmoothRainbow::stepSize too big, quarter and threeQuarter will overlap");

    // Change one of the colors if they reach (close to) zero.
    if (switchingColor != ColorTransition::None ||
        ((currCycleStep != constrain(currCycleStep, quarter, quarter + stepSize - 1U)) &&
         (currCycleStep != constrain(currCycleStep, threeQuarter, threeQuarter + stepSize - 1U))))
    {
        transitionColor = getTransitionColor();
        return;
    }

    // TODO: Write a blink function for debugging purposes

    const auto giveNextColor{[&]() {
        auto colorCandidate{NeoColor::ColorList[random16(6U)]};
        while (colorCandidate == colorA || colorCandidate == colorB)
        {
            colorCandidate = NeoColor::ColorList[random16(6U)];
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
    transitionColor = getTransitionColor();
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

    constexpr CrestParts partList[]{CentreBody, LeftWing, RightWing};

    for (uint8_t led_index{part.start_range}; led_index <= part.end_range; led_index++)
    {
        led_strip.rgb_list[led_index] = NeoColor::blue;
        led_strip.Show();
        delay(250U);
    }
}