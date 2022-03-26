#include "agitator_reverse_command.hpp"

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"

#include "tap/communication/gpio/leds.hpp"
namespace control
{
namespace agitator
{
void AgitatorReverseCommand::initialize() { agitator->initialize(); }

void AgitatorReverseCommand::execute() { agitator->setDesiredOutput(-1000.0f); }

void AgitatorReverseCommand::end(bool) { agitator->setDesiredOutput(0.0f); }

bool AgitatorReverseCommand::isFinished() const { return false; }
}  // namespace agitator
}  // namespace control
