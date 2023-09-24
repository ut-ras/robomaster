#include "command_sentry_aim.hpp"

namespace commands
{
void CommandSentryAim::initialize() {}

void CommandSentryAim::execute() { turret->inputManualAngles(turret->getChassisYaw(), 0.0f); }

void CommandSentryAim::end(bool) {}

bool CommandSentryAim::isFinished(void) const { return false; }
}  // namespace commands
