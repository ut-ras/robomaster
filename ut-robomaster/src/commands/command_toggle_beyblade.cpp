#include "command_toggle_beyblade.hpp"

namespace commands
{
void CommandToggleBeyblade::initialize() { state->isBeyblading = !state->isBeyblading; }
void CommandToggleBeyblade::execute() {}
void CommandToggleBeyblade::end(bool) {}
bool CommandToggleBeyblade::isFinished() const { return true; }
}  // namespace command