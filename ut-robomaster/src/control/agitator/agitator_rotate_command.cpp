#include "agitator_rotate_command.hpp"

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"
namespace control
{
namespace agitator
{
void AgitatorRotateCommand::initialize() {}

void AgitatorRotateCommand::execute() { agitator->setDesiredRPM(2000); }

void AgitatorRotateCommand::end(bool) { agitator->setDesiredRPM(0); }

bool AgitatorRotateCommand::isFinished() const { return false; }
}  // namespace agitator
}  // namespace control
