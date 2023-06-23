#include "command_unjam.hpp"

namespace commands
{
void CommandUnjam::initialize() { shooter->setUnjamming(true); }

void CommandUnjam::execute() {}

void CommandUnjam::end(bool) { shooter->setUnjamming(false); }

bool CommandUnjam::isFinished() const { return false; }
}  // namespace commands