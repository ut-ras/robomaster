#include "command_burst_agitator.hpp"

namespace commands
{
void CommandRotateAgitatorBurst::initialize()
{
    isLeftTurret = !isLeftTurret;
    initialPosition = isLeftTurret ? agitator->getLeftPosition() : agitator->getRightPosition();
}

void CommandRotateAgitatorBurst::execute()
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)

    float ballsPerSecondConst = BALLS_PER_SEC;

    float ballsPerSecondLeft = 0.0f;
    float ballsPerSecondRight = 0.0f;

    float agitatorCapacity = BALLS_PER_REV;  // CONSTANT

    if (drivers->refSerial.getRefSerialReceivingData())
    {
        if (isLeftTurret)
        {
            ballsPerSecondLeft = ballsPerSecondConst;
        }
        else
        {
            ballsPerSecondRight = ballsPerSecondConst;
        }
        targetPosition =
            numToFire / agitatorCapacity;  // calculate based on number of balls to fire
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

void CommandRotateAgitatorBurst::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandRotateAgitatorBurst::isFinished() const
{
    float deltaPosition = isLeftTurret ? agitator->getLeftPosition() - initialPosition
                                       : agitator->getRightPosition() - initialPosition;
    if (deltaPosition >= targetPosition)
    {
        agitator->setBallsPerSecond(0, 0);
        return true;
    }

    // ID1 = Left, ID2 = Right

    int heatLim = isLeftTurret ? drivers->refSerial.getRobotData().turret.heatLimit17ID1
                               : drivers->refSerial.getRobotData().turret.heatLimit17ID2;
    int currHeat = isLeftTurret ? drivers->refSerial.getRobotData().turret.heat17ID1
                                : drivers->refSerial.getRobotData().turret.heat17ID2;

    int buffer = BARREL_HEAT_BUFFER;
    if (currHeat >= heatLim - buffer)
    {
        return true;
    }

    return false;
}
}  // namespace commands