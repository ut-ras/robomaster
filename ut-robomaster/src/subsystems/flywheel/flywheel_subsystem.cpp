#include "flywheel_subsystem.hpp"

namespace subsystems::flywheel
{
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motor_L{drivers, FLYWHEEL_L},
      motor_R{drivers, FLYWHEEL_R}
{
    // Note: this subsystem needs to be registered inside `standard_control.hpp`
}

void FlywheelSubsystem::initialize()
{
    // Initialize motors here
}

void FlywheelSubsystem::refresh()
{
    bool killSwitch = drivers->isKillSwitched();  // killSwitch is true when the controller is off

    // Enable/disable motors and update their velocities here
}

void FlywheelSubsystem::setVelocity(float newVelocity) { velocity = newVelocity; }

bool FlywheelSubsystem::isActive() { return velocity != 0.0f; }
}  // namespace subsystems::flywheel
