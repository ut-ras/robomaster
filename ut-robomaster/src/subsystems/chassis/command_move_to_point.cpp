#include "command_move_chassis.hpp"

namespace commands
{
void CommandMoveChassis::initialize() { keyboardInputMove = Vector2f(0.0f); }

void CommandMoveChassis::execute()
{
    Vector2f target;
    Vector2f current;

    Vector2f movementVector = target - current;

    while (movementVector.getLength() > 0.1f)
    {
        Vector2f direction = movementVector.normalized();
        chassis->input(direction, 0.0f);

        current;  // Update current position here
        movementVector = target - current;
    }

    chassis->input(Vector2f(0.0f), 0.0f);
}

void CommandMoveChassis::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassis::isFinished() const { return false; }

bool CommandMoveChassis::applyKeyboardInput(Vector2f& inputMove, float& inputSpin)
{
    Remote* remote = &drivers->remote;

    inputSpin = 0.0f;  // no keyboard spin controls

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

bool CommandMoveChassis::applyJoystickInput(Vector2f& inputMove, float& inputSpin)
{
    Remote* remote = &drivers->remote;

    inputMove = Vector2f(
        remote->getChannel(Remote::Channel::RIGHT_HORIZONTAL),
        remote->getChannel(Remote::Channel::RIGHT_VERTICAL));

    inputSpin = remote->getChannel(Remote::Channel::WHEEL);

    drivers->rtt << "Chassis Input Move: (" << inputMove.getX() << ", " << inputMove.getY()
                 << ") Spin: " << inputSpin << "\n";

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