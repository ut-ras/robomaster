#include "shooter_on_command.hpp"

#include "tap/control/command.hpp"

#include "shooter_subsystem.hpp"

#include "tap/communication/gpio/leds.hpp"
namespace subsystems
{
namespace shooter
{
void ShooterOnCommand::initialize() {}

void ShooterOnCommand::execute() { 
    shooter->setFlywheelOutput(0.5);
    // shooter->setAgitatorOutput(1000);
    shooter->rotateAgitatorToTarget();
}

void ShooterOnCommand::end(bool) { 
    shooter->setFlywheelOutput(0); 
    shooter->setAgitatorOutput(0);
}

bool ShooterOnCommand::isFinished() const { return false; }
}  // namespace shooter
}  // namespace control