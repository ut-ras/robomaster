#include "agitator_subsystem.hpp"

namespace subsystems::agitator
{

AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      agitator{drivers, M2006, ID_AGITATOR, CAN_AGITATOR, false, "agitator", PID_AGITATOR}
{
}

void AgitatorSubsystem::initialize() { agitator.initialize(); }

void AgitatorSubsystem::refresh()
{
    bool killSwitch = drivers->isKillSwitched();
    agitator.setActive(!killSwitch);
    agitator.update(speed);
}

void AgitatorSubsystem::setSpeed(float newSpeed) { speed = newSpeed; }

}  // namespace subsystems::agitator
