#include "command_move_chassis_joystick.hpp"

namespace commands
{
void CommandMoveChassisJoystick::initialize() {}

void CommandMoveChassisJoystick::execute()
{
    if (drivers->bmi088.getImuState() == ImuInterface::ImuState::IMU_CALIBRATING)
    {
        chassis->input(Vector2f(0.0f), 0.0f);
    }
    else
    {
        Remote* remote = &drivers->remote;

        inputMove = Vector2f(
            remote->getChannel(Remote::Channel::RIGHT_HORIZONTAL),
            remote->getChannel(Remote::Channel::RIGHT_VERTICAL));

        inputSpin = static_cast<float>(remote->getWheel()) / 660.0f;  // 660 is the max

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

        chassis->input(inputMove, inputSpin);
    }
}

void CommandMoveChassisJoystick::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassisJoystick::isFinished() const { return false; }
}  // namespace commands