#include "command_unjam_agitator.hpp"

namespace commands
{
void CommandUnjamAgitator::initialize() {}

void CommandUnjamAgitator::execute() {
    agitator->setBallsPerSecond(UNJAM_SPEED);
}

void CommandUnjamAgitator::end(bool) {
    agitator->setBallsPerSecond(0.0f);
}

bool CommandUnjamAgitator::isFinished() const { return false; }
}  // namespace commands