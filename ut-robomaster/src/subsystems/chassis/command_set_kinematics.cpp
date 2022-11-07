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
    float vX = drivers->remote.getChannel(Remote::Channel::LEFT_HORIZONTAL);
    float vY = -(drivers->remote.getChannel(Remote::Channel::LEFT_VERTICAL));
    float wZ = -(drivers->remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL));

    if (vX * vX + vY * vY < analogDeadZone * analogDeadZone)
    {
        vX = 0.0f;
        vY = 0.0f;
    }

    if (abs(wZ) < analogDeadZone)
    {
        wZ = 0.0f;
    }

    Remote* remote = &drivers->remote;

    if (remote->keyPressed(Remote::Key::A))
    {
        vX -= 1.0f;
    }

    if (remote->keyPressed(Remote::Key::D))
    {
        vX += 1.0f;
    }

    if (remote->keyPressed(Remote::Key::W))
    {
        vY += 1.0f;
    }

    if (remote->keyPressed(Remote::Key::S))
    {
        vY -= 1.0f;
    }

    if (remote->keyPressed(Remote::Key::R) != isRKeyPressed)
    {
        isRKeyPressed = remote->keyPressed(Remote::Key::R);
        if (isRKeyPressed) subsystem->isBeyblading = !subsystem->isBeyblading;
    }

    if (subsystem->isBeyblading)
    {
        vX *= subsystem->TRANSLATION_TO_ROTATION_RATIO;
        vY *= subsystem->TRANSLATION_TO_ROTATION_RATIO;

        wZ = (1 - subsystem->TRANSLATION_TO_ROTATION_RATIO);
    }

    subsystem->setVelocities(vX, vY, wZ);
}

void SetKinematicsCommand::end(bool) { subsystem->setVelocities(0.0f, 0.0f, 0.0f); }

bool SetKinematicsCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace subsystems