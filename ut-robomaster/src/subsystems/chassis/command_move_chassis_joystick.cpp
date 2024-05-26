#include "command_move_chassis_joystick.hpp"

#include "utils/chassis_auto_align.hpp"

namespace commands
{
void CommandMoveChassisJoystick::initialize() {}

void CommandMoveChassisJoystick::execute()
{
    Remote* remote = &drivers->remote;

    Vector2f inputMove(
        remote->getChannel(Remote::Channel::RIGHT_HORIZONTAL),
        remote->getChannel(Remote::Channel::RIGHT_VERTICAL));

    float inputSpin = remote->getWheel() / 660.0f;  // 660 is the max

    float inputMoveLen = inputMove.getLength();
    if (inputMoveLen < ANALOG_DEAD_ZONE)
    {
        inputMove = Vector2f(0.0f);
    }
    else
    {
        inputMove /= max(1.0f, inputMoveLen);  // clamp length
    }

    if (abs(inputSpin) < ANALOG_DEAD_ZONE)
    {
        inputSpin = 0.0f;
    }

    // apply quadratic input ramping
    inputMove *= inputMove.getLength();
    inputSpin *= abs(inputSpin);

    if (turretRelative)
    {
        float yawAngle = turret->getTargetLocalYaw();

        // auto-align to turret when moving
        if (inputMove.getLengthSquared() > 0.0f && inputSpin == 0.0f)
        {
            inputSpin = calculateAutoAlignCorrection(yawAngle, CHASSIS_AUTOALIGN_ANGLE) *
                        CHASSIS_AUTOALIGN_FACTOR;
        }

        chassis->input(Vector2f(inputMove).rotate(yawAngle), inputSpin);
    }
    else
    {
        chassis->input(inputMove, inputSpin);
    }
}

void CommandMoveChassisJoystick::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassisJoystick::isFinished() const { return false; }
}  // namespace commands