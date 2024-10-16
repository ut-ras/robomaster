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
    src::Drivers* drivers,
    FlywheelSubsystem* flywheel,
    MotorConfig motor)
    : Subsystem(drivers),
      drivers(drivers),
      flywheel(flywheel),
      agitator{drivers, motor}
{
}
#elif defined(TARGET_HERO)
AgitatorSubsystem::AgitatorSubsystem(
    src::Drivers* drivers,
    FlywheelSubsystem* flywheel,
    MotorConfig motor)
    : Subsystem(drivers),
      drivers(drivers),
      flywheel(flywheel),
      agitator{drivers, motor},
      feeder{drivers, FEEDER}
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
#ifdef DEMO_MODE
    return;
#endif

    float time = getTimeMilliseconds() / 1000.0f;
    float velocity = getShapedVelocity(time, 1.0f, 0.0f, ballsPerSecond);
    bool killSwitch = drivers->isKillSwitched() || !flywheel->isActive();

    agitator.setActive(!killSwitch);
    agitator.updateVelocity(velocity);
#ifdef TARGET_HERO
    feeder.setActive(!killSwitch);
    feeder.updateVelocity(velocity * FEEDER_RATIO);
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
float AgitatorSubsystem::getVelocity() { return agitator.measureVelocity(); }
}  // namespace subsystems::agitator
