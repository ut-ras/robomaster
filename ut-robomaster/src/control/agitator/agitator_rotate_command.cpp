#include "agitator_rotate_command.hpp"

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"
namespace control
{
namespace agitator
{
void AgitatorRotateCommand::initialize() { agitator->initialize(); }

void AgitatorRotateCommand::execute() { agitator->setDesiredOutput(1000.0f); }

void AgitatorRotateCommand::end(bool) { agitator->setDesiredOutput(0.0f); }

bool AgitatorRotateCommand::isFinished() const { return false; }
}  // namespace agitator
}  // namespace control
