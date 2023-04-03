#include "agitator_subsystem.hpp"

namespace subsystems::shooter
{

AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motor{drivers, M2006, ID_AGITATOR, CAN_SHOOTER, false, "agitator", PID_AGITATOR}
{
}

void AgitatorSubsystem::initialize() { motor.initialize(); }

void AgitatorSubsystem::refresh() { motor.update(isShooting ? SPEED : 0.0f); }

void AgitatorSubsystem::setShooting(bool shooting) { isShooting = shooting; }

}  // namespace subsystems::shooter
