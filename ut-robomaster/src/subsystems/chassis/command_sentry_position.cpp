#include "command_sentry_position.hpp"

namespace commands
{

void CommandSentryPosition::initialize() { startup_time = getTimeMilliseconds(); }

void CommandSentryPosition::execute()
{
    Vector2f move = Vector2f(0.0f);
    float spin = 0.0f;

    if (drivers->isGameActive() && !gameStarted)
    {
        gameStarted = true;
        startup_time = getTimeMilliseconds();
    }

    // if (prevTime == 0.0f)
    // {
    //     prev_time = getTimeMilliseconds();
    // }

    // if (getTimeMilliseconds() - prev_time > 5000.0f)
    // {
    //     direction *= -1;
    //     prev_time = getTimeMilliseconds();
    // }

    // if (!isStarted)
    // {
    //     isStarted = true;
    //     startTime = getTimeMilliseconds() / 1000.0f;
    // }

    // float time = getTimeMilliseconds() / 1000.0f - startTime;

    // if (time < 2.0f)
    // {
    //     float t = time / 2.0f;
    //     float fac = 4.0f * (1.0f - t) * t;
    //     move = Vector2f(0.0f, fac * 0.1f);
    // }
    // else if (time < 3.0f)
    // {
    //     spin = 0.2f;
    // }

    spin = 1.0f;

    if (gameStarted && getTimeMilliseconds() - startup_time < 35000.0f)
    {
        spin = 0.0f;
    }

    // don't move for 10 sec
    if (gameStarted && getTimeMilliseconds() - startup_time < 10000.f)
    {
        move = Vector2f(0.0f);
    }

    if (drivers->isGameActive())
    {
        chassis->input(move, spin);
    }
    else
    {
        chassis->input(Vector2f(0.0f), 0.0f);
    }
}

void CommandSentryPosition::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }
bool CommandSentryPosition::isFinished() const { return false; }

}  // namespace commands
