#include "command_set_kinematics.hpp"

float sq(float x) { return x * x; }

namespace subsystems
{
namespace chassis
{
void SetKinematicsCommand::initialize() {}

void SetKinematicsCommand::execute()
{
    Remote* remote = &drivers->remote;
    Remote::SwitchState rightSwitchState = remote->getSwitch(Remote::Switch::RIGHT_SWITCH);

    if (rightSwitchState == Remote::SwitchState::UP)  // controller input mode
    {
        Vector2f moveInput = Vector2f(
            remote->getChannel(Remote::Channel::LEFT_HORIZONTAL),
            -remote->getChannel(Remote::Channel::LEFT_VERTICAL));
        float spinInput = -remote->getChannel(Remote::Channel::RIGHT_HORIZONTAL);

        velocity = moveInput.getLengthSquared() > sq(analogDeadZone) ? moveInput : Vector2f(0.0f);
        wZ = (abs(wZ) > analogDeadZone) ? spinInput : 0.0f;
    }
    else  // keyboard input mode
    {
        if (remote->keyPressed(Remote::Key::R) != isRKeyPressed)
        {
            isRKeyPressed = remote->keyPressed(Remote::Key::R);
            if (isRKeyPressed) subsystem->isBeyblading = !subsystem->isBeyblading;
        }

        Vector2f input = Vector2f(0.0f);

        if (remote->keyPressed(Remote::Key::A))
        {
            input.x -= 1.0f;
        }

        if (remote->keyPressed(Remote::Key::D))
        {
            input.x += 1.0f;
        }

        if (remote->keyPressed(Remote::Key::W))
        {
            input.y += 1.0f;
        }

        if (remote->keyPressed(Remote::Key::S))
        {
            input.y -= 1.0f;
        }

        velocity += input * moveAccel * DELTA_TIME;

        // clamp velocities
        velocity /= modm::max(1.0f, velocity.getLengthSquared());

        // decelerate
        float velLen = velocity.getLength();
        if (velLen > 0.0f)
        {
            velocity *= max(1.0f - moveDecel * DELTA_TIME / velLen, 0.0f);
        }
    }

    if (subsystem->isBeyblading)
    {
        velocity *= subsystem->TRANSLATION_TO_ROTATION_RATIO;
        wZ = 1 - subsystem->TRANSLATION_TO_ROTATION_RATIO;
    }

    subsystem->setVelocities(velocity.x, velocity.y, wZ);
}

void SetKinematicsCommand::end(bool) { subsystem->setVelocities(0.0f, 0.0f, 0.0f); }

bool SetKinematicsCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace subsystems