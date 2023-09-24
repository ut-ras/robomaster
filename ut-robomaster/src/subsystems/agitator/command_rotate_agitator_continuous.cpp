#include "command_rotate_agitator_continuous.hpp"

namespace commands
{
void CommandRotateAgitatorContinuous::initialize() {}

void CommandRotateAgitatorContinuous::execute()
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    float ballsPerSecondLeft = BALLS_PER_SEC;
    float ballsPerSecondRight = BALLS_PER_SEC;

    if (drivers->refSerial.getRefSerialReceivingData())
    {
        if (drivers->refSerial.getRobotData().turret.heat17ID1 >=
            drivers->refSerial.getRobotData().turret.heatLimit17ID1 - BARREL_HEAT_BUFFER)
        {
            ballsPerSecondLeft = 0.0f;
        }

        if (drivers->refSerial.getRobotData().turret.heat17ID2 >=
            drivers->refSerial.getRobotData().turret.heatLimit17ID2 - BARREL_HEAT_BUFFER)
        {
            ballsPerSecondRight = 0.0f;
        }
    }

    agitator->setBallsPerSecond(ballsPerSecondLeft, ballsPerSecondRight);

#elif defined(TARGET_HERO)
    float ballsPerSecond = BALLS_PER_SEC;

    if (drivers->refSerial.getRefSerialReceivingData())
    {
        if (drivers->refSerial.getRobotData().turret.heat42 >=
            drivers->refSerial.getRobotData().turret.heatLimit42 - BARREL_HEAT_BUFFER)
        {
            ballsPerSecond = 0.0f;
        }
    }

    agitator->setBallsPerSecond(ballsPerSecond);
#endif
}

void CommandRotateAgitatorContinuous::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandRotateAgitatorContinuous::isFinished() const { return false; }
}  // namespace commands