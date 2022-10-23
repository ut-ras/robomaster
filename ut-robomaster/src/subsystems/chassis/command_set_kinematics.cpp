#include "command_set_kinematics.hpp"

#include "tap/control/command.hpp"

#include "chassis_subsystem.hpp"

namespace subsystems
{
namespace chassis
{
void SetKinematicsCommand::initialize() {}

void SetKinematicsCommand::execute() { subsystem->setVelocities(0.0f, 0.0f, 1.0f); }

void SetKinematicsCommand::end(bool) { subsystem->setVelocities(0.0f, 0.0f, 0.0f); }

bool SetKinematicsCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace subsystems