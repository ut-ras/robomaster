#include "command_sentry_position.hpp"

namespace subsystems
{
namespace chassis
{

void CommandSentryPosition::initialize() {}

void CommandSentryPosition::execute()
{
    Vector2f move = Vector2f(0.0f);
    float spin = 0.0f;

    if (drivers->remote.getSwitch(Remote::Switch::RIGHT_SWITCH) == Remote::SwitchState::UP)
    {
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
    }

    chassis->input(move, spin);
}

void CommandSentryPosition::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }
bool CommandSentryPosition::isFinished() const { return false; }

}  // namespace chassis
}  // namespace subsystems