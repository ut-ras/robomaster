#include "flywheel_subsystem.hpp"

namespace subsystems::flywheel
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motors{
          {drivers, FLYWHEEL_TL},
          {drivers, FLYWHEEL_TR},
          {drivers, FLYWHEEL_BL},
          {drivers, FLYWHEEL_BR}}
{
}
#elif defined(TARGET_HERO)
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motors{{drivers, FLYWHEEL_L}, {drivers, FLYWHEEL_R}}
{
}
#endif

void FlywheelSubsystem::initialize()
{
    for (int i = 0; i < FLYWHEELS; i++)
    {
        motors[i].initialize();
    }
}

void FlywheelSubsystem::refresh()
{
    bool killSwitch = drivers->isKillSwitched();

#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    for (int i = 0; i < FLYWHEELS; i++)
    {
        motors[i].setActive(!killSwitch);
    }

    // LEFT FLYWHEELS
    motors[0].update(launchSpeedLeft);
    motors[2].update(launchSpeedLeft);

    // RIGHT FLYWHEELS
    motors[1].update(launchSpeedRight);
    motors[3].update(launchSpeedRight);

#elif defined(TARGET_HERO)
    for (int i = 0; i < FLYWHEELS; i++)
    {
        motors[i].setActive(!killSwitch);
        motors[i].update(launchSpeedHero);
    }
#endif
}

void FlywheelSubsystem::setLaunchSpeed(float speed)
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    launchSpeedLeft = speed;
    launchSpeedRight = speed;
#elif defined(TARGET_HERO)
    launchSpeedHero = speed;
#endif
}

void FlywheelSubsystem::setLaunchSpeed(float leftSpeed, float rightSpeed)
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    launchSpeedLeft = leftSpeed;
    launchSpeedRight = rightSpeed;
#elif defined(TARGET_HERO)
    launchSpeedHero = leftSpeed;
#endif
}
}  // namespace subsystems::flywheel
