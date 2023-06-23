#include "shooter_subsystem.hpp"

namespace subsystems
{
namespace shooter
{

ShooterSubsystem::ShooterSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      drivers(drivers),
      flywheel(drivers),
      agitator(drivers)
{
}

void ShooterSubsystem::registerSubsystems()
{
    drivers->commandScheduler.registerSubsystem(&flywheel);
    drivers->commandScheduler.registerSubsystem(&agitator);
}

void ShooterSubsystem::initialize()
{
    flywheel.initialize();
    agitator.initialize();
}

void ShooterSubsystem::refresh() {}

void ShooterSubsystem::setFiringState(FiringState state)
{
    firingState = state;
    flywheel.setActive(state != FiringState::Idle);
    agitator.setShooting(state == FiringState::Firing);

    drivers->leds.set(tap::gpio::Leds::Blue, state == FiringState::Idle);
    drivers->leds.set(tap::gpio::Leds::Green, state == FiringState::Ready);
    drivers->leds.set(tap::gpio::Leds::Red, state == FiringState::Firing);
}

void ShooterSubsystem::setUnjamming(bool unjam) { agitator.setUnjamming(unjam); }

}  // namespace shooter
}  // namespace subsystems