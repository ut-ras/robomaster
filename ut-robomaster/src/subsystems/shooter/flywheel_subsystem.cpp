#include "flywheel_subsystem.hpp"

namespace subsystems::shooter
{
#if defined TARGET_STANDARD || TARGET_SENTRY
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motors{
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_TL,
           CAN_SHOOTER,
           false,
           "flywheel top left",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_TR,
           CAN_SHOOTER,
           true,
           "flywheel top right",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_BL,
           CAN_SHOOTER,
           true,
           "flywheel bottom left",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_BR,
           CAN_SHOOTER,
           false,
           "flywheel bottom right",
           PID_FLYWHEEL}}
{
}
#elif defined TARGET_HERO
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motors{
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_L,
           CAN_SHOOTER,
           false,
           "flywheel left",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_R,
           CAN_SHOOTER,
           true,
           "flywheel right",
           PID_FLYWHEEL}}
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
    for (int i = 0; i < FLYWHEELS; i++)
    {
        motors[i].update(isActive ? SPEED : 0.0f);
    }
}

void FlywheelSubsystem::setActive(bool active) { isActive = active; }
}  // namespace subsystems::shooter
