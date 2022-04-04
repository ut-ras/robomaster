#include "flywheel_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

using namespace tap;

namespace control
{
namespace flywheel
{
FlywheelSubsystem::FlywheelSubsystem(tap::Drivers *drivers)
        : tap::control::Subsystem(drivers),
        drivers(drivers)
        {
        }
        
void FlywheelSubsystem::initialize() { 
    drivers->pwm.write(0.25f, FLYWHEEL_MOTOR_PIN); }

void FlywheelSubsystem::refresh() {}

void FlywheelSubsystem::setDesiredOutput(float output)
{
    drivers->pwm.write(output, FLYWHEEL_MOTOR_PIN);
    drivers->leds.set(tap::gpio::Leds::Green, true);
}
}
}