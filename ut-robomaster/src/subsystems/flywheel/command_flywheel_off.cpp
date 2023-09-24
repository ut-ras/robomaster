#include "command_flywheel_off.hpp"

namespace commands
{

void CommandFlywheelOff::initialize() {}

void CommandFlywheelOff::execute() { flywheel->setLaunchSpeed(0.0f); }

void CommandFlywheelOff::end(bool) {}

bool CommandFlywheelOff::isFinished() const { return false; }
}  // namespace commands