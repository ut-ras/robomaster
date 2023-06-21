#include "command_unjam.hpp"

namespace commands
{
void CommandUnjam::initialize() { }

void CommandUnjam::execute() {}

void CommandUnjam::end(bool) { }

bool CommandUnjam::isFinished() const { return false; }
}  // namespace commands