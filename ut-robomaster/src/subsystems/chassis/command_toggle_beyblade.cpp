#include "command_toggle_beyblade.hpp"

namespace subsystems
{
namespace chassis
{
void CommandToggleBeyblade::initialize() { state->isBeyblading = !state->isBeyblading; }
void CommandToggleBeyblade::execute() {}
void CommandToggleBeyblade::end(bool) {}
bool CommandToggleBeyblade::isFinished() const { return true; }
}  // namespace chassis
}  // namespace subsystems