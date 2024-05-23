#include "command_rotate_flywheel.hpp"

namespace commands
{

void CommandRotateFlywheel::initialize() { flywheel->setLaunchSpeed(FLYWHEEL_SPEED); }

void CommandRotateFlywheel::execute() {}

void CommandRotateFlywheel::end(bool) { flywheel->setLaunchSpeed(0.0f); }

bool CommandRotateFlywheel::isFinished() const { return false; }
}  // namespace commands