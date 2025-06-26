#include "command_sentry_aim.hpp"

namespace commands
{
void CommandSentryAim::initialize() {}

void CommandSentryAim::execute()
{
    // turret->setTargetWorldAngles(turret->getChassisYaw(), 0.0f);
    // if (turret->getTargetLocalYaw() <= -55.0f)
    // {
    //     turret->setTargetWorldAngles(50.0f, 10.0f);
    // }
    // else
    // {
    // turret->setTargetWorldAngles(-50.0f, -10.0f);
    // }

    if (!drivers->isKillSwitched())
    {
        turret->setTargetWorldAngles(50.0f, 10.0f);
    }

    /*
        PLAN (for suhas):
        - see if setTargetWorldAngles() works as intended
        - create targetWorldYaw/pitch variables
        - twYaw should switch between -200 and 200 depending on what its set to
            - if the angle is -200 set it to 200
            - if the angle is 200 set it to -200
            - else, keep current targetWorldAngle
        - time how long it takes for pitch motor to oscillate
        - use a timer to oscillate pitch to move ~10 deg to scan, or if that doesn't work, use the
        - same logic to scan
    */
}

void CommandSentryAim::end(bool) {}

bool CommandSentryAim::isFinished(void) const { return false; }
}  // namespace commands
