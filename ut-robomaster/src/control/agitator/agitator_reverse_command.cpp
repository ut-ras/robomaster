#include "agitator_reverse_command.hpp"

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"

#include "tap/communication/gpio/leds.hpp"
namespace control
{
namespace agitator
{
void AgitatorReverseCommand::initialize() {}

void AgitatorReverseCommand::execute() { agitator->setDesiredRPM(-2000); }

void AgitatorReverseCommand::end(bool) { agitator->setDesiredRPM(0); }

bool AgitatorReverseCommand::isFinished() const { return false; }
}  // namespace agitator
}  // namespace control
