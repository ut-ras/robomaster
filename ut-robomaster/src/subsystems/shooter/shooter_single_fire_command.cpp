#include "shooter_single_fire_command.hpp"

namespace subsystems
{
namespace shooter
{
void ShooterSingleFireCommand::initialize() {}

void ShooterSingleFireCommand::execute() {}

void ShooterSingleFireCommand::end(bool) {}

bool ShooterSingleFireCommand::isFinished() const { return false; }
}  // namespace shooter
}  // namespace subsystems