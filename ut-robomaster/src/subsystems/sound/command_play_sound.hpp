#pragma once

#include "tap/architecture/periodic_timer.hpp"
#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "sound.hpp"
#include "sound_subsystem.hpp"

namespace commands
{

using subsystems::sound::SoundSubsystem;
using tap::arch::PeriodicMilliTimer;

class CommandPlaySound : public tap::control::Command
{
public:
    CommandPlaySound(src::Drivers *drivers, SoundSubsystem *subsystem, Sound sound)
        : drivers(drivers),
          subsystem(subsystem),
          sound(sound),
          timer(sound.note_interval)
    {
        addSubsystemRequirement(subsystem);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "play sound command"; }

private:
    src::Drivers *drivers;
    SoundSubsystem *subsystem;
    Sound sound;

    uint16_t note = 0;
    PeriodicMilliTimer timer;
};  // class CommandPlaySound
}  // namespace commands