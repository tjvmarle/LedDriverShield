#include "led_strip.hpp"
#include "pattern.hpp"
#include <Arduino.h>

//! @brief Representation of the main class to control the hardware of the shield crest
class ShieldCrest
{
  public:
    ShieldCrest();

    //! @brief Adds a new pattern to the collection.
    // TODO: Should probably use some static storage for these patterns instead of dynamic.
    void addPattern(Pattern* newPattern);

    //! @brief Main function to run the lighting. Displays the current Pattern.
    void Display();

  private:
    // stuff we need:
    // List of patterns. Should be (kinda) dynamic... linked list?
    LedStrip led_strip;
    Pattern* currPattern; //! Storage for a single pattern. Will be played through Display.
    bool runInit{true};
};