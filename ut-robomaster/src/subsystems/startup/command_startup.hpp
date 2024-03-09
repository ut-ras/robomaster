#pragma once

#include "tap/architecture/clock.hpp"
#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "startup_subsystem.hpp"

#define S 0.0f
#define C5 523.25f
#define Cs5 554.37f
#define Ds5 622.25f
#define E5 659.25f
#define F5 698.46f
#define Fs5 739.99f
#define G5 783.99f
#define Gs5 830.61f
#define A5 880.0f
#define C6 1046.5f
#define Cs6 1108.73f

#define NOTE_COUNT 61

namespace commands
{

using subsystems::startup::StartupSubsystem;
using tap::arch::clock::getTimeMilliseconds;

class CommandStartup : public tap::control::Command
{
public:
    CommandStartup(src::Drivers *drivers, StartupSubsystem *startup)
        : drivers(drivers),
          startup(startup)
    {
        addSubsystemRequirement(startup);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "startup command"; }

private:
    src::Drivers *drivers;
    StartupSubsystem *startup;

    uint32_t startTime = 0;
    int lastNote = -1;
    // pacman
    float notes[NOTE_COUNT] = {C5,  S,  C6,  S, G5,  S,   E5, S, C6,  G5,  E5, S, E5, E5, E5, S,
                               Cs5, S,  Cs6, S, Gs5, S,   F5, S, Cs6, Gs5, F5, S, F5, F5, F5, S,
                               C5,  S,  C6,  S, G5,  S,   E5, S, C6,  G5,  E5, S, E5, E5, E5, S,
                               Ds5, E5, F5,  S, F5,  Fs5, G5, S, G5,  Gs5, A5, S, C6};
    float timestep = 880.0f / (1000.0f * 60.0f);  // notes/ms
    bool done = false;
};  // class CommandStartup
}  // namespace commands