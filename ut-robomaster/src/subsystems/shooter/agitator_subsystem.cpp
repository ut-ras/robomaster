#include "agitator_subsystem.hpp"

namespace
{
float shape(float t, float a, float phi)
{
    return (1.0f - a) * cos((2.0f * t + phi) * M_PI) + 1.0f;
}
}  // namespace

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

#if defined TARGET_STANDARD || defined TARGET_SENTRY
    float vel1 = shape(time * BALLS_PER_SEC, 0.0f, 0.0f) / BALLS_PER_REV;
    float vel2 = shape(time * BALLS_PER_SEC, 0.0f, 1.0f) / BALLS_PER_REV;

    leftAgitator.update(isShooting ? vel1 : 0.0f);
    rightAgitator.update(isShooting ? vel2 : 0.0f);
#elif defined TARGET_HERO
    float vel = shape(time * BALLS_PER_SEC, 0.0f, 1.0f) / BALLS_PER_REV;
    agitator.update(isShooting ? vel : 0.0f);
    feeder.update(isShooting ? FEEDER_SPEED : 0.0f);
#endif
}

void AgitatorSubsystem::setShooting(bool shooting)
{
    isShooting = shooting;
    startTime = getTimeMilliseconds();
}

}  // namespace subsystems::shooter
