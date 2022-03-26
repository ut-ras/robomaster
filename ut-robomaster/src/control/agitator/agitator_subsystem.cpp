#include "agitator_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

#include "tap/drivers.hpp"

using namespace tap;

namespace control
{
namespace agitator
{
void AgitatorSubsystem::initialize() { 
    
    agitatorMotor.initialize();
    // drivers->leds.set(tap::gpio::Leds::Red, true);
}

void AgitatorSubsystem::refresh() {}

void AgitatorSubsystem::setDesiredOutput(float output)
{
    agitatorMotor.setDesiredOutput(output);
}

}
}