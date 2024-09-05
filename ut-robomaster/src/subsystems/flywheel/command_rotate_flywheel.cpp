#include "command_rotate_flywheel.hpp"

namespace commands
{
void CommandRotateFlywheel::initialize()
{
    // Start the flywheels here
}

void CommandRotateFlywheel::execute()
{
    // not required for this command
}

void CommandRotateFlywheel::end(bool)
{
    // Stop the flywheels here
}

bool CommandRotateFlywheel::isFinished() const { return false; }
}  // namespace commands