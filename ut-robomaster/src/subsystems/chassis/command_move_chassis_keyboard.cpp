#include "command_move_chassis_keyboard.hpp"

namespace commands
{
void CommandMoveChassisKeyboard::initialize() { inputMove = Vector2f(0.0f); }

void CommandMoveChassisKeyboard::execute()
{
    Remote* remote = &drivers->remote;

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

    float yawAngle = turret->getTargetLocalYaw();
    float inputSpin = 0.0f;

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

void CommandMoveChassisKeyboard::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassisKeyboard::isFinished() const { return false; }
}  // namespace commands