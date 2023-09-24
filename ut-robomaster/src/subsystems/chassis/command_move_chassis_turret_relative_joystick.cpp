#include "command_move_chassis_turret_relative_joystick.hpp"

namespace commands
{
void CommandMoveChassisTurretRelativeJoystick::initialize() {}

void CommandMoveChassisTurretRelativeJoystick::execute()
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

        float turretYaw = turret->getTargetLocalYaw();
        Vector2f turretRelativeMove = Vector2f(inputMove);
        turretRelativeMove.rotate(turretYaw);

        // auto-align to turret when moving
        if (inputMove.getLengthSquared() > 0.0f && inputSpin == 0.0f)
        {
            float deltaAngle =
                turretYaw - roundf(turretYaw / SNAP_ANGLE) * SNAP_ANGLE;  // nearest side
            float correction = deltaAngle / SNAP_ANGLE * 4.0f;
            inputSpin = correction / max(1.0f, abs(correction)) * TURRET_ALIGN_FACTOR;
        }

        chassis->input(turretRelativeMove, inputSpin);
    }
}

void CommandMoveChassisTurretRelativeJoystick::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassisTurretRelativeJoystick::isFinished() const { return false; }
}  // namespace commands