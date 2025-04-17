#include "command_sentry_position.hpp"

namespace commands
{

void CommandSentryPosition::initialize() { moveTimer.stop(); }

void CommandSentryPosition::execute()
{
    Remote *remote = &drivers->remote;

    Vector2f inputMove = Vector2f(
        remote->getChannel(Remote::Channel::RIGHT_HORIZONTAL),
        remote->getChannel(Remote::Channel::RIGHT_VERTICAL));

    inputMove *= -1;

    float inputSpin = remote->getChannel(Remote::Channel::WHEEL);

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

    inputMove *= MAX_LINEAR_VEL;
    inputSpin *= MAX_ANGULAR_VEL;

    chassis->setOmniVelocities(inputMove, inputSpin);

    // if (drivers->isGameActive() && moveTimer.isStopped())
    // {
    //     moveTimer.restart(10'000);  // 10s
    // }

    // if (!moveTimer.isExpired())
    // {
    //     chassis->input(Vector2f(0.0f), 0.0f);
    //     return;
    // }

    // // speen
    // chassis->input(Vector2f(0.0f), 1.0f);
}

void CommandSentryPosition::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }
bool CommandSentryPosition::isFinished() const { return false; }

}  // namespace commands
