#include "shooter_on_command.hpp"

#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command.hpp"

#include "shooter_subsystem.hpp"
namespace subsystems
{
namespace shooter
{
void ShooterOnCommand::initialize() {}

void ShooterOnCommand::execute()
{
    shooter->setFlywheelOutput(0.10);
    shooter->setAgitatorOutput(500);
    // shooter->rotateAgitatorToTarget();
}

void ShooterOnCommand::end(bool)
{
    shooter->setFlywheelOutput(0);
    shooter->setAgitatorOutput(0);
}

bool ShooterOnCommand::isFinished() const { return false; }
}  // namespace shooter
}  // namespace subsystems