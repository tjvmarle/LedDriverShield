#pragma once

#include "crest_mapping.hpp"
#include "led_strip.hpp"
#include <Arduino.h>

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