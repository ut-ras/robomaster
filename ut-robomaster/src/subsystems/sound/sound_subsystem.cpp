#include "sound_subsystem.hpp"
namespace subsystems
{
namespace sound
{
SoundSubsystem::SoundSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      drivers(drivers)
{
}

void SoundSubsystem::initialize() {}

void SoundSubsystem::refresh() {}

void SoundSubsystem::silence() { tap::buzzer::silenceBuzzer(&drivers->pwm); }

void SoundSubsystem::setBuzzerFrequency(int frequency)
{
    tap::buzzer::playNote(&drivers->pwm, frequency);
    // define keys with numerical frequencies
}
};  // namespace sound
}  // namespace subsystems
