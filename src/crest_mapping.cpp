#include "crest_mapping.hpp"

// TODO: Maybe move this to constants instead of having each user require its own instantiation?

MappedPart::MappedPart(CrestParts crest_part, uint8_t start_led, uint8_t end_led)
    : part{crest_part}, start_range{start_led}, end_range{end_led}
{
}

MappedShield::MappedShield()
{
    // Exact mapping of the LEDs to the parts on the shield. The entire thing contrains 196 LEDs.
    PartsList[CrestParts::CentreBody] = MappedPart{CrestParts::CentreBody, 0U, 19U};
    PartsList[CrestParts::LeftClaw] = MappedPart{CrestParts::LeftClaw, 20U, 31U};
    PartsList[CrestParts::LeftWing] = MappedPart{CrestParts::LeftWing, 32U, 98U};
    PartsList[CrestParts::LeftTriangle] = MappedPart{CrestParts::LeftTriangle, 99U, 104U};
    PartsList[CrestParts::TopTriangle] = MappedPart{CrestParts::TopTriangle, 105U, 110U};
    PartsList[CrestParts::RightTriangle] = MappedPart{CrestParts::RightTriangle, 111U, 116U};
    PartsList[CrestParts::RightWing] = MappedPart{CrestParts::RightWing, 117U, 183U};
    PartsList[CrestParts::RightClaw] = MappedPart{CrestParts::RightClaw, 184U, 195U};
    PartsList[CrestParts::All] = MappedPart{CrestParts::All, 0U, 195U};
}

const MappedPart& MappedShield::getPart(CrestParts part)
{
    return this->PartsList[part];
}