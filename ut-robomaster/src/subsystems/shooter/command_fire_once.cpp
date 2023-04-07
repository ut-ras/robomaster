#include "command_fire_once.hpp"

namespace subsystems
{
namespace shooter
{
void CommandFireOnce::initialize() {}

void CommandFireOnce::execute() {}

void CommandFireOnce::end(bool) {}

bool CommandFireOnce::isFinished() const { return false; }
}  // namespace shooter
}  // namespace subsystems