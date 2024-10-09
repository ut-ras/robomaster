#include "command_move_chassis.hpp"

namespace commands
{
void CommandMoveChassis::initialize() { inputMove = Vector2f(0.0f); }

void CommandMoveChassis::execute()
{
    Remote* remote = &drivers->remote;
    // Keyboard
    Vector2f rawMoveInput = Vector2f(
        remote->keyPressed(Remote::Key::D) - remote->keyPressed(Remote::Key::A),
        remote->keyPressed(Remote::Key::W) - remote->keyPressed(Remote::Key::S));

    float rawInputLen = rawMoveInput.getLength();

    if (rawInputLen > 0.0f)
    {
        Vector2f moveDir = rawMoveInput / rawInputLen;  // normalize input
        inputMove += moveDir * KEYBOARD_ACCEL * DT;     // incorporate input
        inputMove /= max(1.0f, inputMove.getLength());  // clamp length
    }
    else
    {
        // decelerate when input stops
        float len = inputMove.getLength();
        if (len > 0.0f)
        {
            inputMove *= max(1.0f - KEYBOARD_DECEL * DT / len, 0.0f);
        }
    }

    float inputSpin = 0.0f;
    if (keyboardTurretRelative)
    {
        float yawAngle = turret->getTargetLocalYaw();

        if (beyblade)
        {
            inputSpin = 1.0f;
        }
        else if (inputMove.getLengthSquared() > 0.0f)  // auto-align
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

    // Only take joystick input when no keyboard input and not beyblading
    if (inputMove.getLengthSquared() > 0.0f && inputSpin == 0.0f)
    {
        inputMove = Vector2f(
            remote->getChannel(Remote::Channel::RIGHT_HORIZONTAL),
            remote->getChannel(Remote::Channel::RIGHT_VERTICAL));

        inputSpin = remote->getChannel(Remote::Channel::WHEEL);

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

        if (joystickTurretRelative)
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
}

void CommandMoveChassis::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassis::isFinished() const { return false; }
}  // namespace commands