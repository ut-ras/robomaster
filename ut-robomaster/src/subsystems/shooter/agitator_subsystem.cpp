#include "agitator_subsystem.hpp"

namespace subsystems::shooter
{

#if defined TARGET_STANDARD || defined TARGET_SENTRY
AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      leftMotor{drivers, M2006, ID_AGITATOR_L, CAN_SHOOTER, false, "agitator left", PID_AGITATOR},
      rightMotor{drivers, M2006, ID_AGITATOR_R, CAN_SHOOTER, true, "agitator right", PID_AGITATOR}
{
}
#elif defined TARGET_HERO
AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motor{drivers, M2006, ID_AGITATOR, CAN_SHOOTER, false, "agitator", PID_AGITATOR}
{
}
#endif

void AgitatorSubsystem::initialize()
{
#if defined TARGET_STANDARD || defined TARGET_SENTRY
    leftMotor.initialize();
    rightMotor.initialize();
#elif defined TARGET_HERO
    motor.initialize();
#endif
}

void AgitatorSubsystem::refresh()
{
#if defined TARGET_STANDARD || defined TARGET_SENTRY
    leftMotor.update(isShooting ? SPEED : 0.0f);
    rightMotor.update(isShooting ? SPEED : 0.0f);
#elif defined TARGET_HERO
    motor.update(isShooting ? SPEED : 0.0f);
#endif
}

void AgitatorSubsystem::setShooting(bool shooting) { isShooting = shooting; }

}  // namespace subsystems::shooter
