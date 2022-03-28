#include "agitator_reverse_command.hpp"

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"

#include "tap/communication/gpio/leds.hpp"
namespace control
{
namespace agitator
{
void AgitatorReverseCommand::initialize() {}

void AgitatorReverseCommand::execute() { agitator->setDesiredOutput(-700.0f); }

void AgitatorReverseCommand::end(bool) { agitator->setDesiredOutput(0.0f); }

bool AgitatorReverseCommand::isFinished() const { return false; }
}  // namespace agitator
}  // namespace control
