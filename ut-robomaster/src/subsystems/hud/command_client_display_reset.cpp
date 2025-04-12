#include "command_client_display_reset.hpp"

#include "tap/communication/sensors/buzzer/buzzer.hpp"

namespace commands
{
void CommandClientDisplayReset::initialize() { tap::buzzer::playNote(&drivers->pwm, 440); }

void CommandClientDisplayReset::execute() {}

void CommandClientDisplayReset::end(bool) { tap::buzzer::silenceBuzzer(&drivers->pwm); }

bool CommandClientDisplayReset::isFinished() const { return false; }
}  // namespace commands