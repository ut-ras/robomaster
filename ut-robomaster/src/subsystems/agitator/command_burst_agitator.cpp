#include "command_burst_agitator.hpp"

namespace commands
{
void CommandRotateAgitatorBurst::initialize() { numToFire = 8; }

void CommandRotateAgitatorBurst::execute()
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    float ballsPerSecondLeftConst = BALLS_PER_SEC;
    float ballsPerSecondRightConst = BALLS_PER_SEC;

    float ballsPerSecondLeft = 0.0f;
    float ballsPerSecondRight = 0.0f;
    uint32_t beginTime = 0;
    uint32_t deltaTime = -1;
    uint32_t time = 0;
    cooldownMeter.heatLimit1 = drivers->refSerial.getRobotData().turret.heatLimit17ID1;
    cooldownMeter.heatLimit2 = drivers->refSerial.getRobotData().turret.heatLimit17ID2;
    cooldownMeter.currentHeat1 = drivers->refSerial.getRobotData().turret.heat17ID1;
    cooldownMeter.currentHeat2 = drivers->refSerial.getRobotData().turret.heat17ID2;
    bool isTurretOne = cooldownMeter.compare() >= 0 ? true : false;

    if (drivers->refSerial.getRefSerialReceivingData())
    {
        if (isTurretOne)
        {
            ballsPerSecondLeft = ballsPerSecondLeftConst;
            ballsPerSecondRight = 0;
            time = 1000 * getNumToFire() /
                   ballsPerSecondLeft;  // time in milliseconds that agitator must be run for
        }
        else
        {
            ballsPerSecondLeft = 0;
            ballsPerSecondRight = ballsPerSecondRightConst;
            time = 1000 * getNumToFire() /
                   ballsPerSecondRight;  // time in milliseconds that agitator must be run for
        }
    }

    beginTime = tap::arch::clock::getTimeMilliseconds();
    agitator->setBallsPerSecond(ballsPerSecondLeftConst, ballsPerSecondRightConst);

    // delay
    while (deltaTime < time)
    {
        deltaTime = tap::arch::clock::getTimeMilliseconds() - beginTime;
    }

    agitator->setBallsPerSecond(0, 0);

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

bool CommandRotateAgitatorBurst::isFinished() const { return false; }
}  // namespace commands