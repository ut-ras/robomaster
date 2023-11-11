#include "command_burst_agitator.hpp"

namespace commands
{
void CommandRotateAgitatorBurst::initialize()
{
    numToFire = 8;
    beginTime = tap::arch::clock::getTimeMilliseconds();
}

void CommandRotateAgitatorBurst::execute()
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    float ballsPerSecondLeftConst = BALLS_PER_SEC;
    float ballsPerSecondRightConst = BALLS_PER_SEC;

    float ballsPerSecondLeft = 0.0f;
    float ballsPerSecondRight = 0.0f;
    // cooldownMeter.heatLimit1 = drivers->refSerial.getRobotData().turret.heatLimit17ID1;
    // cooldownMeter.heatLimit2 = drivers->refSerial.getRobotData().turret.heatLimit17ID2;
    // cooldownMeter.currentHeat1 = drivers->refSerial.getRobotData().turret.heat17ID1;
    // cooldownMeter.currentHeat2 = drivers->refSerial.getRobotData().turret.heat17ID2;
    bool isTurretOne = cooldownMeter.compare() >= 0 ? true : false;

    if (drivers->refSerial.getRefSerialReceivingData())
    {
        // if (isTurretOne)
        // {
        ballsPerSecondLeft = ballsPerSecondLeftConst;
        ballsPerSecondRight = 0;
        time = 1000 * getNumToFire() /
               ballsPerSecondLeft;  // time in milliseconds that agitator must be run for
        // }
        // else
        // {
        //     ballsPerSecondLeft = 0;
        //     ballsPerSecondRight = ballsPerSecondRightConst;
        //     time = 1000 * getNumToFire() /
        //            ballsPerSecondRight;  // time in milliseconds that agitator must be run
        //            for
        // }
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

int CommandRotateAgitatorBurst::getNumToFire() { return numToFire; }

void CommandRotateAgitatorBurst::setNumToFire(int num) { numToFire = num; }

void CommandRotateAgitatorBurst::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandRotateAgitatorBurst::isFinished() const
{
    int deltaTime = deltaTime = tap::arch::clock::getTimeMilliseconds() - beginTime;
    if (deltaTime >= time)
    {
        agitator->setBallsPerSecond(0, 0);
        return true;
    }
    return false;
}
}  // namespace commands