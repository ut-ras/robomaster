#include "agitator_subsystem.hpp"

namespace subsystems::agitator
{

using tap::algorithms::compareFloatClose;
using tap::arch::clock::getTimeMilliseconds;

/**
 * AgitatorSubsystem class instantiation
 */
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
AgitatorSubsystem::AgitatorSubsystem(
    src::Drivers *drivers,
    MotorId agitatorMotorId,
    bool motorFlipped)
    : Subsystem(drivers),
      drivers(drivers),
      agitator{drivers, M2006, agitatorMotorId, CAN_SHOOTER, motorFlipped, "agitator", PID_AGITATOR}
{
}
#elif defined(TARGET_HERO)
AgitatorSubsystem::AgitatorSubsystem(
    src::Drivers *drivers,
    MotorId agitatorMotorId,
    bool motorFlipped)
    : Subsystem(drivers),
      drivers(drivers),
      agitator{
          drivers,
          M3508,
          agitatorMotorId,
          CAN_SHOOTER,
          motorFlipped,
          "agitator",
          PID_AGITATOR},
      feeder{drivers, M2006, ID_FEEDER, CAN_SHOOTER, false, "feeder", PID_FEEDER}
{
}
#endif

void AgitatorSubsystem::initialize()
{
    agitator.initialize();
#ifdef TARGET_HERO
    feeder.initialize();
#endif
}

void AgitatorSubsystem::refresh()
{
    float time = getTimeMilliseconds() / 1000.0f;
    float velocity = getShapedVelocity(time, 1.0f, 0.0f, ballsPerSecond);
    bool killSwitch = drivers->isKillSwitched();

    agitator.setActive(!killSwitch);
    agitator.update(velocity);
#ifdef TARGET_HERO
    feeder.setActive(!killSwitch);
    feeder.update(velocity * FEEDER_RATIO);
#endif
}

float AgitatorSubsystem::getShapedVelocity(float time, float a, float phi, float ballsPerSecond)
{
    float t = time * ballsPerSecond;
    float maxVel = ballsPerSecond / BALLS_PER_REV;
    return ((1.0f - a) * cos((2.0f * t + phi) * M_PI) + 1.0f) * maxVel;
}

void AgitatorSubsystem::setBallsPerSecond(float bps) { ballsPerSecond = bps; }
float AgitatorSubsystem::getPosition() { return agitator.measurePosition(); }
}  // namespace subsystems::agitator
