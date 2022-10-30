#include "shooter_single_fire_command.hpp"

#include "tap/control/command.hpp"

#include "shooter_subsystem.hpp"

#include "tap/communication/gpio/leds.hpp"
namespace subsystems
{
namespace shooter
{
void ShooterSingleFireCommand::initialize() {}

void ShooterSingleFireCommand::execute() { 
    shooter->shootBalls(2);
}

void ShooterSingleFireCommand::end(bool) { 
}

bool ShooterSingleFireCommand::isFinished() const { return false; }
}  // namespace shooter
}  // namespace control