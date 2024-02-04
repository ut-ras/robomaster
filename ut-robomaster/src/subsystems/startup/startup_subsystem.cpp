#include "startup_subsystem.hpp"
namespace subsystems
{
namespace startup
{
StartupSubsystem::StartupSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      drivers(drivers)
{
}

void StartupSubsystem::refresh() {}

void StartupSubsystem::silence() { tap::buzzer::silenceBuzzer(&drivers->pwm); }

void StartupSubsystem::setBuzzerFrequency(int frequency)
{
    tap::buzzer::playNote(&drivers->pwm, frequency);
    // define keys with numerical frequencies
}
};  // namespace startup
}  // namespace subsystems
