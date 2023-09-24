#include "agitator_subsystem.hpp"

namespace subsystems::agitator
{
using tap::algorithms::compareFloatClose;
using tap::arch::clock::getTimeMilliseconds;

#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
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

#elif defined(TARGET_HERO)
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
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    leftAgitator.initialize();
    rightAgitator.initialize();

#elif defined(TARGET_HERO)
    agitator.initialize();
    feeder.initialize();
#endif
}

void AgitatorSubsystem::refresh()
{
    float time = getTimeMilliseconds() / 1000.0f;  // MAY BREAK ON WRAPPING!
    bool killSwitch = drivers->isKillSwitched();

#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    leftAgitator.setActive(!killSwitch);
    rightAgitator.setActive(!killSwitch);

    leftAgitator.update(getShapedVelocity(time, 1.0f, 0.0f, ballsPerSecondLeft));
    rightAgitator.update(getShapedVelocity(time, 1.0f, 1.0f, ballsPerSecondRight));

#elif defined(TARGET_HERO)
    agitator.setActive(!killSwitch);
    feeder.setActive(!killSwitch);

    agitator.update(getShapedVelocity(time, 1.0f, 1.0f, ballsPerSecondHero));
    feeder.update(compareFloatClose(ballsPerSecondHero, 0.0f, 1E-6) ? 0.0f : FEEDER_SPEED);
#endif
}

float AgitatorSubsystem::getShapedVelocity(float time, float a, float phi, float ballsPerSecond)
{
    float t = time * ballsPerSecond;
    float maxVel = ballsPerSecond / BALLS_PER_REV;
    return ((1.0f - a) * cos((2.0f * t + phi) * M_PI) + 1.0f) * maxVel;
}

void AgitatorSubsystem::setBallsPerSecond(float bps)
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    ballsPerSecondLeft = bps;
    ballsPerSecondRight = bps;

#elif defined(TARGET_HERO)
    ballsPerSecondHero = bps;
#endif
}

void AgitatorSubsystem::setBallsPerSecond(float bpsLeft, float bpsRight)
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    ballsPerSecondLeft = bpsLeft;
    ballsPerSecondRight = bpsRight;

#elif defined(TARGET_HERO)
    ballsPerSecondHero = bpsLeft;
#endif
}

}  // namespace subsystems::agitator
