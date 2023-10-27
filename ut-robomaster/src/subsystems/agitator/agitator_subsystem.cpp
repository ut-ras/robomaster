#include "agitator_subsystem.hpp"

namespace subsystems::agitator
{

AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      agitator1{drivers, M2006, tap::motor::MOTOR2, CAN_AGITATOR, false, "agitator1", PID_AGITATOR},
      agitator2{drivers, M3508, tap::motor::MOTOR6, CAN_AGITATOR, false, "agitator2", PID_AGITATOR}
{
}

void AgitatorSubsystem::initialize()
{
    agitator1.initialize();
    agitator2.initialize();
}

void AgitatorSubsystem::refresh()
{
    bool killSwitch = drivers->isKillSwitched();
    agitator1.setActive(!killSwitch);
    agitator1.update(speed);
    agitator2.setActive(!killSwitch);
    agitator2.update(speed);
}

void AgitatorSubsystem::setSpeed(float newSpeed) { speed = newSpeed; }

}  // namespace subsystems::agitator
