#include "flywheel_on_command.hpp"

#include "tap/control/command.hpp"

#include "flywheel_subsystem.hpp"

#include "tap/communication/gpio/leds.hpp"
namespace control
{
namespace flywheel
{
void FlywheelOnCommand::initialize() {
}

void FlywheelOnCommand::execute() { flywheel->setDesiredOutput(1.0f); tap::gpio::Leds led; led.set(tap::gpio::Leds::Green, true); }

void FlywheelOnCommand::end(bool) { flywheel->setDesiredOutput(0.0f); }

bool FlywheelOnCommand::isFinished() const { return false; }
}  // namespace flywheel
}  // namespace control
