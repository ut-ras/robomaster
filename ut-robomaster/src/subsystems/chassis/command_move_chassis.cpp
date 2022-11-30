#include "command_move_chassis.hpp"

namespace subsystems
{
namespace chassis
{
void MoveChassisCommand::initialize() {}

void MoveChassisCommand::execute()
{
    if (drivers->bmi088.getImuState() == ImuInterface::ImuState::IMU_CALIBRATING)
    {
        subsystem->input(Vector2f(0.0f), 0.0f);
    }
    else
    {
        bool useControllerInput =
            drivers->remote.getSwitch(Remote::Switch::RIGHT_SWITCH) == Remote::SwitchState::UP;

        if (useControllerInput)
            doControllerInput();
        else
            doKeyboardInput();

        subsystem->input(inputMove, inputSpin);
    }
}

void MoveChassisCommand::end(bool) { subsystem->input(Vector2f(0.0f), 0.0f); }
bool MoveChassisCommand::isFinished() const { return false; }

void MoveChassisCommand::doControllerInput()
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

    // apply quadratic input ramping
    inputMove *= inputMove.getLength();
    inputSpin *= abs(inputSpin);
}

void MoveChassisCommand::doKeyboardInput()
{
    Remote* remote = &drivers->remote;
    bool isRKeyPressed = remote->keyPressed(Remote::Key::R);
    if (isRKeyPressed != wasRKeyPressed)
    {
        wasRKeyPressed = isRKeyPressed;
        if (isRKeyPressed)
        {
            isBeyblading = !isBeyblading;
        }
    }

    if (isBeyblading)
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