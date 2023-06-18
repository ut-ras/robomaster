#include "agitator_subsystem.hpp"

namespace subsystems::shooter
{
using tap::arch::clock::getTimeMilliseconds;

#if defined TARGET_STANDARD || defined TARGET_SENTRY
AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      leftAgitator{
          drivers,
          M2006,
          ID_AGITATOR_L,
          CAN_SHOOTER,
          false,
          "agitator left",
          PID_AGITATOR},
      rightAgitator{
          drivers,
          M2006,
          ID_AGITATOR_R,
          CAN_SHOOTER,
          true,
          "agitator right",
          PID_AGITATOR}
{
}
#elif defined TARGET_HERO
AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      agitator{drivers, M3508, ID_AGITATOR, CAN_SHOOTER, false, "agitator", PID_AGITATOR},
      feeder{drivers, M2006, ID_FEEDER, CAN_SHOOTER, false, "feeder", PID_FEEDER}
{
}
#endif

void AgitatorSubsystem::initialize()
{
#if defined TARGET_STANDARD || defined TARGET_SENTRY
    leftAgitator.initialize();
    rightAgitator.initialize();
#elif defined TARGET_HERO
    agitator.initialize();
    feeder.initialize();
#endif
}

void AgitatorSubsystem::refresh()
{
    float time = getTimeMilliseconds() / 1000.0f;  // MAY BREAK ON WRAPPING!
    bool killSwitch = drivers->isKillSwitched();

#if defined TARGET_STANDARD || defined TARGET_SENTRY
    leftAgitator.setActive(!killSwitch);
    rightAgitator.setActive(!killSwitch);

    if (isUnjamming)
    {
        leftAgitator.update(-UNJAM_SPEED);
        rightAgitator.update(-UNJAM_SPEED);
    }
    else if (isShooting)
    {
        bool isShootingLeft = true;
        bool isShootingRight = true;

        if (drivers->refSerial.getRefSerialReceivingData())
        {
            if (drivers->refSerial.getRobotData().turret.heat17ID1 >=
                drivers->refSerial.getRobotData().turret.heatLimit17ID1 - BARREL_HEAT_BUFFER)
            {
                isShootingLeft = false;
            }

            if (drivers->refSerial.getRobotData().turret.heat17ID2 >=
                drivers->refSerial.getRobotData().turret.heatLimit17ID2 - BARREL_HEAT_BUFFER)
            {
                isShootingRight = false;
            }
        }

        leftAgitator.update(isShootingLeft ? getShapedVelocity(time, 1.0f, 0.0f) : 0.0f);
        rightAgitator.update(isShootingRight ? getShapedVelocity(time, 1.0f, 1.0f) : 0.0f);
    }
    else
    {
        leftAgitator.update(0.0f);
        rightAgitator.update(0.0f);
    }
#elif defined TARGET_HERO
    agitator.setActive(!killSwitch);
    feeder.setActive(!killSwitch);

    agitator.update(isShooting ? getShapedVelocity(time, 1.0f, 1.0f) : 0.0f);
    feeder.update(isShooting ? FEEDER_SPEED : 0.0f);
#endif
}

float AgitatorSubsystem::getShapedVelocity(float time, float a, float phi)
{
    float t = time * BALLS_PER_SEC;
    float maxVel = BALLS_PER_SEC / BALLS_PER_REV;
    return ((1.0f - a) * cos((2.0f * t + phi) * M_PI) + 1.0f) * maxVel;
}

void AgitatorSubsystem::setShooting(bool shooting)
{
    isShooting = shooting;
    startTime = getTimeMilliseconds();
}

void AgitatorSubsystem::setUnjamming(bool unjamming) { isUnjamming = unjamming; }

}  // namespace subsystems::shooter
