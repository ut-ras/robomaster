#include "command_move_chassis.hpp"

namespace commands
{
void CommandMoveChassis::initialize() { keyboardInputMove = Vector2f(0.0f); }

void CommandMoveChassis::execute()
{
    float yawAngle = turret->getTargetLocalYaw();
    Vector2f inputMove = Vector2f(0.0f);
    float inputSpin = 0.0f;

    // get keyboard input
    float keyboardInputSpin;
    bool hasKeyboardInput = applyKeyboardInput(keyboardInputMove, keyboardInputSpin);

    // get joystick input
    Vector2f joystickInputMove;
    float joystickInputSpin;
    bool hasJoystickInput = applyJoystickInput(joystickInputMove, joystickInputSpin);

    // decide which input source to use (keyboard has inertia)
    if (!hasKeyboardInput && hasJoystickInput)
    {
        inputMove = joystickInputMove;
        inputSpin = joystickInputSpin;
    }
    else
    {
        inputMove = keyboardInputMove;
        inputSpin = keyboardInputSpin;
    }

    // auto-align chassis to turret when moving
    if (inputMove.getLengthSquared() > 0.0f && inputSpin == 0.0f)
    {
        inputSpin = calculateAutoAlignCorrection(yawAngle, CHASSIS_AUTOALIGN_ANGLE) *
                    CHASSIS_AUTOALIGN_FACTOR;
    }

    // override spin input while beyblading
    if (beyblade)
    {
        inputSpin = 0.5f;
    }

    // rotate movement vector relative to turret
    if (turretRelative)
    {
        inputMove = inputMove.rotate(yawAngle);
    }

    chassis->input(inputMove, inputSpin);
}

void CommandMoveChassis::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassis::isFinished() const { return false; }

bool CommandMoveChassis::applyKeyboardInput(Vector2f &inputMove, float &inputSpin)
{
    Remote *remote = &drivers->remote;

    if (remote->keyPressed(Remote::Key::E)) {
        inputSpin = 0.5f;
    } else if (remote->keyPressed(Remote::Key::Q)) {
        inputSpin = -0.5f;
    } else {
        inputSpin = 0.0f;
    }
    // inputSpin = 0.0f;  // no keyboard spin controls

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

    return rawMoveInput != Vector2f(0.0f);
}

bool CommandMoveChassis::applyJoystickInput(Vector2f &inputMove, float &inputSpin)
{
    Remote *remote = &drivers->remote;

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

    return inputMove != Vector2f(0.0f) || inputSpin != 0.0f;
}

}  // namespace commands