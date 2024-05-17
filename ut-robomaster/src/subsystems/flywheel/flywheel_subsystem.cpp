#include "flywheel_subsystem.hpp"

namespace subsystems::flywheel
{
#if defined(TARGET_STANDARD) || defined(TARGET_HERO)
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motors{{drivers, FLYWHEEL_L}, {drivers, FLYWHEEL_R}}
{
}
#elif defined(TARGET_SENTRY)
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

    for (int i = 0; i < FLYWHEELS; i++)
    {
        motors[i].setActive(!killSwitch);
        motors[i].update(launchSpeed);
    }
}

void FlywheelSubsystem::setLaunchSpeed(float speed) { launchSpeed = speed; }
}  // namespace subsystems::flywheel
