#include "command_move_chassis.hpp"

namespace subsystems
{
namespace chassis
{

#define SNAP_ANGLE M_PI

void CommandMoveChassis::initialize() {}

void CommandMoveChassis::execute()
{
    if (drivers->bmi088.getImuState() == ImuInterface::ImuState::IMU_CALIBRATING)
    {
        chassis->input(Vector2f(0.0f), 0.0f);
    }
    else
    {
        bool turretRelative = true;

        switch (drivers->remote.getSwitch(Remote::Switch::RIGHT_SWITCH))
        {
            case Remote::SwitchState::DOWN:
                doKeyboardInput();
                break;
            case Remote::SwitchState::MID:
                doControllerInput();
                turretRelative = false;
                break;
            default:
                doControllerInput();
                break;
        }

        if (turretRelative)
        {
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
        else
        {
            chassis->input(inputMove, inputSpin);
        }
    }
}

void CommandMoveChassis::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }
bool CommandMoveChassis::isFinished() const { return false; }

void CommandMoveChassis::doControllerInput()
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
}

void CommandMoveChassis::doKeyboardInput()
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

    inputSpin = isBeyblading ? 1.0f : 0.0f;

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