#include "sound_subsystem.hpp"

#include "tap/communication/sensors/buzzer/buzzer.hpp"

namespace subsystems::sound
{
using tap::gpio::Pwm;

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
}
}  // namespace subsystems::sound
