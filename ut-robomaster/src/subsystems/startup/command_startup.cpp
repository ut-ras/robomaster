#include "command_startup.hpp"

namespace commands
{

void CommandStartup::initialize() { startup->setBuzzerFrequency(110); }

void CommandStartup::execute() {}

void CommandStartup::end(bool) {}

bool CommandStartup::isFinished() const { return false; }

}  // namespace commands