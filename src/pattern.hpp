#pragma once

#include "crest_mapping.hpp"
#include "led_strip.hpp"
#include <Arduino.h>

// Global reference of colors for the patterns to use.
namespace NeoColor
{
constexpr uint8_t maxLedIntensity{255U};
constexpr Neopixel red{maxLedIntensity, 0U, 0U};
constexpr Neopixel green{0U, maxLedIntensity, 0U};
constexpr Neopixel blue{0U, 0U, maxLedIntensity};
constexpr Neopixel cyan{0U, maxLedIntensity, maxLedIntensity};
constexpr Neopixel magenta{maxLedIntensity, 0U, maxLedIntensity};
constexpr Neopixel yellow{maxLedIntensity, maxLedIntensity, 0U};
constexpr Neopixel ColorList[]{red, green, blue, cyan, magenta, yellow}; //! R,G,B,C,M,Y
} // namespace NeoColor

//! @brief Abstract base class to write some pretty color-patterns
class Pattern
{
  protected:
    uint8_t speed{100U};
    MappedShield shield{}; //! Not required for all patterns. Just for ease of access.
    LedStrip& led_strip;
    const uint8_t num_leds;

  public:
    Pattern(LedStrip& leds);
    virtual ~Pattern(){};

    //! @brief Initialize the pattern. Can be used to give the pattern a startup sequence before continuing via a loop.
    virtual void Init() = 0;

    //! @brief Play the pattern.
    virtual void Play() = 0;

    //! @brief Change the speed of the pattern.
    //! @param[in]  speed  Play speed of the pattern. 100 is default speed, 100%. 200 is double speed.
    virtual void SetSpeed(uint8_t speed);
};

class SmoothRainbow : public Pattern
{
  private:
    enum class ColorTransition
    {
        A,
        B,
        None
    };

    static constexpr uint16_t cycleSize{1000U}; //! Max length of the main cycle.
    uint16_t currCycleStep{0U};                 //! Tracks progression on the main cycle.
    static constexpr uint16_t stepSize{20U};    //! Increment size of currCycleStep
    Neopixel colorA{}, colorB{}, nextColor{};
    bool aSwitched{false}, bSwitched{false};

    ColorTransition switchingColor{ColorTransition::None};

    //! @brief            Set a specific part of the crest to rainbow colors. All parts run on the same cycle.
    //! @param[in]  part  Part of the crest to set the LED colors for.
    void newRainbow(CrestParts part);

  public:
    SmoothRainbow(LedStrip& leds);

    //! @brief Initialize the pattern.
    void Init() override;

    //! @brief Plays a smooth moving rainbow color.
    void Play() override;
};

class DemoPattern : public Pattern
{
  public:
    DemoPattern(LedStrip& leds);

    //! @brief Initialize the pattern.
    void Init() override;

    //! @brief Plays a smooth moving rainbow color.
    void Play() override;
};