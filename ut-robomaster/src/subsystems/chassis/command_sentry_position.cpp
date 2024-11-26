#include "command_sentry_position.hpp"

namespace commands
{

void CommandSentryPosition::initialize() { moveTimer.stop(); }

void CommandSentryPosition::execute()
{
    // wait until game starts then wait some more to avoid excess drifting
    if (drivers->isGameActive() && moveTimer.isStopped())
    {
        moveTimer.restart(10'000);  // 10s
    }

    if (!moveTimer.isExpired())
    {
        chassis->input(Vector2f(0.0f), 0.0f);
        return;
    }

    // speen
    chassis->input(Vector2f(0.0f), 1.0f);
}

void CommandSentryPosition::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }
bool CommandSentryPosition::isFinished() const { return false; }

}  // namespace commands
