#include "flywheel_subsystem.hpp"

#include "tap/drivers.hpp"

namespace subsystems::shooter
{
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motor{drivers, M2006, ID_FLYWHEEL, CAN_SHOOTER, false, "flywheel", PID_FLYWHEEL}
{
}

void FlywheelSubsystem::initialize() { motor.initialize(); }

void FlywheelSubsystem::refresh() { motor.update(isActive ? SPEED : 0.0f); }

void FlywheelSubsystem::setActive(bool active) { isActive = active; }
}  // namespace subsystems::shooter
