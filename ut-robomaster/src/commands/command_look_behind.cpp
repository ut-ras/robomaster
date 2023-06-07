#include "command_look_behind.hpp"

namespace commands
{
void CommandLookBehind::initialize() { state->isLookingBehind = !state->isLookingBehind; }
void CommandLookBehind::execute() {}
void CommandLookBehind::end(bool) {}
bool CommandLookBehind::isFinished() const { return true; }
}  // namespace command