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
    shooter->setDesiredRPM(1200); 
    
}

void ShooterOnCommand::end(bool) { shooter->setDesiredRPM(0); }

bool ShooterOnCommand::isFinished() const { return false; }
}  // namespace shooter
}  // namespace control