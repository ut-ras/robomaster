#include "command_set_kinematics.hpp"

namespace subsystems
{
namespace chassis
{
void SetKinematicsCommand::initialize() {}

void SetKinematicsCommand::execute()
{
    if (drivers->bmi088.getImuState() == ImuInterface::ImuState::IMU_CALIBRATING && false)
    {
        subsystem->setVelocities(Vector2f(0.0f), 0.0f);
    }
    else
    {
        bool useControllerInput =
            drivers->remote.getSwitch(Remote::Switch::RIGHT_SWITCH) == Remote::SwitchState::UP;

        if (useControllerInput)
            doControllerInput();
        else
            doKeyboardInput();

        // effective inputs after correction
        Vector2f effMoveInput = inputMove;
        float effSpinInput = inputSpin;

        // overinput error
        float magMove = inputMove.getLength();
        float magSpin = abs(inputSpin);
        float e = max(0.0f, magMove + magSpin - 1.0f);

        if (e > 0.0f)
        {
            effMoveInput *= 1 - e * SPIN_FACTOR / magMove;
            effSpinInput *= 1 - e * (1.0f - SPIN_FACTOR) / magSpin;
        }

        // convert inputs to velocities
        Vector2f moveVel = effMoveInput * LINEAR_VELOCITY_MAX;
        float spinVel = effSpinInput * ANGULAR_VELOCITY_MAX;

        subsystem->setVelocities(moveVel, spinVel);
    }
}

void SetKinematicsCommand::end(bool) { subsystem->setVelocities(Vector2f(0.0f), 0.0f); }
bool SetKinematicsCommand::isFinished() const { return false; }

void SetKinematicsCommand::doControllerInput()
{
    Remote* remote = &drivers->remote;
    inputMove = Vector2f(
        remote->getChannel(Remote::Channel::RIGHT_HORIZONTAL),
        -remote->getChannel(Remote::Channel::RIGHT_VERTICAL));
    inputSpin = -static_cast<float>(remote->getWheel()) / 660.0f;  // 660 is the max

    float inputMoveLen = inputMove.getLength();
    if (inputMoveLen < ANALOG_DEAD_ZONE)
    {
        inputMove = Vector2f(0.0f);
    }
    else
    {
        inputMove /= max(1.0f, inputMove.getLength());  // clamp length
    }

    if (abs(inputSpin) < ANALOG_DEAD_ZONE)
    {
        inputSpin = 0.0f;
    }
}

void SetKinematicsCommand::doKeyboardInput()
{
    Remote* remote = &drivers->remote;
    bool isRKeyPressed = remote->keyPressed(Remote::Key::R);
    if (isRKeyPressed != wasRKeyPressed)
    {
        wasRKeyPressed = isRKeyPressed;
        if (isRKeyPressed)
        {
            subsystem->isBeyblading = !subsystem->isBeyblading;
        }
    }

    if (subsystem->isBeyblading)
    {
        inputSpin = 1.0f;
    }

    Vector2f rawMoveInput = Vector2f(
        remote->keyPressed(Remote::Key::D) - remote->keyPressed(Remote::Key::A),
        remote->keyPressed(Remote::Key::W) - remote->keyPressed(Remote::Key::S));
    float rawInputLen = rawMoveInput.getLength();

    if (rawInputLen > 0.0f)
    {
        Vector2f moveDir = rawMoveInput / rawInputLen;       // normalize input
        inputMove += moveDir * KEYBOARD_ACCEL * DELTA_TIME;  // incorporate input
        inputMove /= max(1.0f, inputMove.getLength());       // clamp length
    }
    else
    {
        // decelerate when input stops
        float len = inputMove.getLength();
        if (len > 0.0f)
        {
            inputMove *= max(1.0f - KEYBOARD_DECEL * DELTA_TIME / len, 0.0f);
        }
    }
}
}  // namespace chassis
}  // namespace subsystems