#include "command_set_kinematics_keyboard.hpp"

#include "tap/drivers.hpp"

using namespace tap::communication::serial;

namespace subsystems
{
namespace chassis
{
void SetKinematicsKeyboardCommand::initialize() {}

void SetKinematicsKeyboardCommand::execute()
{
    float x = drivers->remote.keyPressed(Remote::Key::D) - drivers->remote.keyPressed(Remote::Key::A);
    float y = -(drivers->remote.keyPressed(Remote::Key::W) - drivers->remote.keyPressed(Remote::Key::S));
    float z = -(drivers->remote.keyPressed(Remote::Key::Q) - drivers->remote.keyPressed(Remote::Key::E));
    subsystem->setVelocities(x, y, z);
}

void SetKinematicsKeyboardCommand::end(bool) { subsystem->setVelocities(0.0f, 0.0f, 0.0f); }

bool SetKinematicsKeyboardCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace subsystems