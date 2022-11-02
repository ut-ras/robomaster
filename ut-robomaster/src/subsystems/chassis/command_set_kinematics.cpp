#include "command_set_kinematics.hpp"

#include "tap/drivers.hpp"

using namespace tap::communication::serial;

namespace subsystems
{
namespace chassis
{
void SetKinematicsCommand::initialize() {}

void SetKinematicsCommand::execute()
{
    float x = drivers->remote.getChannel(Remote::Channel::LEFT_HORIZONTAL);
    float y = -(drivers->remote.getChannel(Remote::Channel::LEFT_VERTICAL));
    float z = -(drivers->remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL));
    subsystem->setVelocities(x, y, z);
}

void SetKinematicsCommand::end(bool) { subsystem->setVelocities(0.0f, 0.0f, 0.0f); }

bool SetKinematicsCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace subsystems