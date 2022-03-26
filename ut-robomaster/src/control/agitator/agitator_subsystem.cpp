#include "agitator_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

#include "tap/drivers.hpp"

using namespace tap;

namespace control
{
namespace agitator
{
void AgitatorSubsystem::initialize() { 
    drivers->leds.set(tap::gpio::Leds::Green, true);
    agitatorMotor.initialize();
}

void AgitatorSubsystem::refresh() {}

void AgitatorSubsystem::setDesiredOutput(float output)
{
    agitatorMotor.setDesiredOutput(output);
}

void AgitatorSubsystem::setLed()
{
    while (1)
    {
        drivers->leds.set(tap::gpio::Leds::Green, true);
    }
}
}
}