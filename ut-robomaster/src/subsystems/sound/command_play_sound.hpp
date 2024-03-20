#pragma once

#include "tap/architecture/timeout.hpp"
#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "sound.hpp"
#include "sound_subsystem.hpp"

namespace commands
{

using subsystems::sound::SoundSubsystem;
using tap::arch::MilliTimeout;

class CommandPlaySound : public tap::control::Command
{
public:
    CommandPlaySound(
        src::Drivers *drivers,
        SoundSubsystem *subsystem,
        Sound sound,
        bool loop = false)
        : drivers(drivers),
          subsystem(subsystem),
          sound(sound),
          loop(loop),
          timer()
    {
        addSubsystemRequirement(subsystem);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "play sound command"; }

private:
    void playNote(uint8_t note);

    src::Drivers *drivers;
    SoundSubsystem *subsystem;
    Sound sound;

    int read_index = 0;
    bool loop = false;
    MilliTimeout timer;
};  // class CommandPlaySound
}  // namespace commands