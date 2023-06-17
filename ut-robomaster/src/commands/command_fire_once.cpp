#include "command_fire_once.hpp"

namespace commands
{
void CommandFireOnce::initialize() {}
void CommandFireOnce::execute() {}
void CommandFireOnce::end(bool) {}
bool CommandFireOnce::isFinished() const { return false; }
}  // namespace commands